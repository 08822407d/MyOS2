#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - fallback to a repository VMDK/OVMF path is announced before launch
# open_questions: []
# ---

set -Eeuo pipefail
_script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
source "$_script_dir/scripts/common.sh"
source "$_script_dir/scripts/prepare_env.sh"

usage() {
  cat <<'EOF'
Usage: dbg-qemu.sh [--gdb-wait] [--no-gdb-wait]

Configuration:
  MYOS2_DISK_IMAGE       disk image; fallback: <repo>/myos2.vmdk
  MYOS2_DISK_FORMAT      vmdk/qcow2/raw; inferred from extension with warning
  MYOS2_OVMF_CODE        OVMF code image
  MYOS2_OVMF_VARS        OVMF vars template
  MYOS2_QEMU_ACCEL       auto|kvm|tcg (default auto)
  MYOS2_GDB_PORT         default 8864
  MYOS2_QEMU_MEMORY      default 8192
  MYOS2_QEMU_CPUS        default 1
  MYOS2_QEMU_DISPLAY     default gtk; use none for -nographic
EOF
}

resolve_existing() {
  local env_value="$1"
  shift
  local candidate
  if [[ -n "$env_value" ]]; then
    myos2_require_regular_file "$env_value"
    printf '%s\n' "$env_value"
    return
  fi
  for candidate in "$@"; do
    if [[ -f "$candidate" ]]; then
      myos2_warn "configuration unset; auto-selected $candidate"
      printf '%s\n' "$candidate"
      return
    fi
  done
  return 1
}

gdb_wait=1
case "${1:-}" in
  -h|--help) usage; exit 0 ;;
  --gdb-wait|"") gdb_wait=1 ;;
  --no-gdb-wait) gdb_wait=0 ;;
  *) usage >&2; myos2_die "unknown argument: $1" ;;
esac

myos2_require_cmd qemu-system-x86_64
disk="$(resolve_existing "${MYOS2_DISK_IMAGE:-}" "$MYOS2_ROOT/myos2.vmdk")" ||
  myos2_die "set MYOS2_DISK_IMAGE"
ovmf_code="$(resolve_existing "${MYOS2_OVMF_CODE:-}" \
  "$MYOS2_ROOT_MYLOADER/OVMF_CODE.fd" \
  /usr/share/OVMF/OVMF_CODE.fd \
  /usr/share/edk2/x64/OVMF_CODE.fd)" ||
  myos2_die "set MYOS2_OVMF_CODE"
ovmf_vars_template="$(resolve_existing "${MYOS2_OVMF_VARS:-}" \
  "$MYOS2_ROOT_MYLOADER/OVMF_VARS.fd" \
  /usr/share/OVMF/OVMF_VARS.fd \
  /usr/share/edk2/x64/OVMF_VARS.fd)" ||
  myos2_die "set MYOS2_OVMF_VARS"

format="${MYOS2_DISK_FORMAT:-}"
if [[ -z "$format" ]]; then
  case "${disk##*.}" in
    vmdk|VMDK) format=vmdk ;;
    qcow2|QCOW2) format=qcow2 ;;
    img|raw|RAW) format=raw ;;
    *) myos2_die "set MYOS2_DISK_FORMAT" ;;
  esac
  myos2_warn "MYOS2_DISK_FORMAT unset; inferred $format"
fi

accel="${MYOS2_QEMU_ACCEL:-auto}"
if [[ "$accel" == auto ]]; then
  if [[ -r /dev/kvm && -w /dev/kvm ]]; then
    accel=kvm
  else
    accel=tcg
    myos2_warn "/dev/kvm unavailable; using TCG"
  fi
fi
case "$accel" in
  kvm) accel_args=(-enable-kvm -cpu "${MYOS2_QEMU_CPU:-host}") ;;
  tcg) accel_args=(-accel tcg -cpu "${MYOS2_QEMU_CPU:-max}") ;;
  *) myos2_die "MYOS2_QEMU_ACCEL must be auto, kvm, or tcg" ;;
esac

gdb_port="${MYOS2_GDB_PORT:-8864}"
pidfile="${MYOS2_QEMU_PIDFILE:-$MYOS2_ROOT/out/qemu.pid}"
vars_out="${MYOS2_OVMF_VARS_OUT:-$MYOS2_ROOT/out/OVMF_VARS.fd}"
mkdir -p -- "$(dirname -- "$pidfile")" "$(dirname -- "$vars_out")"
[[ -f "$vars_out" ]] || cp -f -- "$ovmf_vars_template" "$vars_out"

if [[ -f "$pidfile" ]]; then
  old_pid="$(cat "$pidfile" 2>/dev/null || true)"
  if [[ -n "$old_pid" ]] && kill -0 "$old_pid" 2>/dev/null; then
    myos2_die "QEMU already appears active: pid=$old_pid"
  fi
  rm -f -- "$pidfile"
fi

qemu_args=(
  "${accel_args[@]}"
  -machine q35
  -m "${MYOS2_QEMU_MEMORY:-8192}"
  -smp "${MYOS2_QEMU_CPUS:-1}"
  -drive "if=pflash,format=raw,readonly=on,file=$ovmf_code"
  -drive "if=pflash,format=raw,file=$vars_out"
  -drive "file=$disk,format=$format,if=virtio"
  -gdb "tcp::$gdb_port,server,nowait"
  -net none
  -serial mon:stdio
)
(( gdb_wait == 1 )) && qemu_args+=(-S)
if [[ "${MYOS2_QEMU_DISPLAY:-gtk}" == none ]]; then
  qemu_args+=(-nographic)
else
  qemu_args+=(-display "${MYOS2_QEMU_DISPLAY:-gtk}")
fi

qemu_pid=""
cleanup() {
  if [[ -n "$qemu_pid" ]] && kill -0 "$qemu_pid" 2>/dev/null; then
    kill "$qemu_pid" 2>/dev/null || true
  fi
  rm -f -- "$pidfile"
}
trap cleanup EXIT INT TERM

qemu-system-x86_64 "${qemu_args[@]}" &
qemu_pid=$!
printf '%s\n' "$qemu_pid" > "$pidfile"

ready=0
for ((i = 0; i < 200; i++)); do
  kill -0 "$qemu_pid" 2>/dev/null || myos2_die "QEMU exited before GDB became ready"
  if command -v nc >/dev/null 2>&1; then
    nc -z 127.0.0.1 "$gdb_port" >/dev/null 2>&1 && ready=1
  else
    (echo >/dev/tcp/127.0.0.1/"$gdb_port") >/dev/null 2>&1 && ready=1
  fi
  (( ready == 1 )) && break
  sleep 0.05
done
(( ready == 1 )) || myos2_die "GDB port did not become ready: $gdb_port"
myos2_info "QEMU READY pid=$qemu_pid gdb=127.0.0.1:$gdb_port accel=$accel"

status=0
wait "$qemu_pid" || status=$?
qemu_pid=""
exit "$status"
