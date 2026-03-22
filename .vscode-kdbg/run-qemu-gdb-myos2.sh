#!/usr/bin/env bash
set -euo pipefail

GDB_PORT="${GDB_PORT:-8864}"
PIDFILE="${PIDFILE:-$PWD/.qemu-myos2.pid}"
USE_KVM="${USE_KVM:-1}"
CPU_OPTS="${CPU_OPTS:-}"
OVMF_DIR="${OVMF_DIR:-$PWD/myloader}"
OVMF_VARS_OUT="${OVMF_VARS_OUT:-$PWD/.vscode-kdbg/ovmf-vars-myos2.fd}"
DISPLAY_OPTS="${DISPLAY_OPTS:--display gtk}"

QEMU_BIN="qemu-system-x86_64"
OVMF_CODE="${OVMF_DIR}/OVMF_CODE.fd"
OVMF_VARS_TEMPLATE="${OVMF_DIR}/OVMF_VARS.fd"
DISK_IMG="$HOME/vmware/myos2test/myos2test-flat.vmdk"

if [[ -z "${CPU_OPTS:-}" ]]; then
  if [[ "$USE_KVM" == "1" ]]; then
    CPU_OPTS="host,+invtsc,kvm=on"
  else
    CPU_OPTS="max"
  fi
fi

read -r -a DISPLAY_ARGS <<< "${DISPLAY_OPTS}"

log_start() { printf 'qemu.task:0: QEMU_START\n'; }
log_ready() { printf 'qemu.task:0: READY\n'; }
log_fail()  { printf 'qemu.task:0: FAIL %s\n' "$*"; }

cleanup_old() {
  if [[ -f "$PIDFILE" ]]; then
    local oldpid
    oldpid="$(cat "$PIDFILE" 2>/dev/null || true)"
    if [[ -n "${oldpid:-}" ]] && kill -0 "$oldpid" 2>/dev/null; then
      kill "$oldpid" 2>/dev/null || true
      sleep 0.2
      kill -9 "$oldpid" 2>/dev/null || true
    fi
    rm -f "$PIDFILE" || true
  fi
}
die() { log_fail "$*"; exit 1; }

cleanup_old

[[ -f "$OVMF_CODE" ]] || die "OVMF_CODE not found: $OVMF_CODE"
[[ -f "$OVMF_VARS_TEMPLATE" ]] || die "OVMF_VARS template not found: $OVMF_VARS_TEMPLATE"
[[ -e "$DISK_IMG" ]] || die "Disk image not found: $DISK_IMG"

mkdir -p "$(dirname "$OVMF_VARS_OUT")"
if [[ ! -f "$OVMF_VARS_OUT" ]]; then
  cp -f "$OVMF_VARS_TEMPLATE" "$OVMF_VARS_OUT"
fi

KVM_ARGS=()
if [[ "$USE_KVM" == "1" ]]; then
  KVM_ARGS=(-enable-kvm)
fi

log_start

"$QEMU_BIN" \
  "${KVM_ARGS[@]}" \
  -machine "pc-q35-noble,i8042=on" \
  -m "8192" \
  -smp "1" \
  -cpu "$CPU_OPTS" \
  -drive "if=pflash,format=raw,readonly=on,file=${OVMF_CODE}" \
  -drive "if=pflash,format=raw,file=${OVMF_VARS_OUT}" \
  -gdb "tcp::${GDB_PORT},server,nowait" \
  -S \
  -drive "file=${DISK_IMG},if=virtio,format=raw" \
  -net none \
  -serial mon:stdio \
  "${DISPLAY_ARGS[@]}" \
  &

qemu_pid=$!
echo "$qemu_pid" > "$PIDFILE"

ready=0
for _ in $(seq 1 200); do
  if ! kill -0 "$qemu_pid" 2>/dev/null; then
    die "QEMU exited early (pid=$qemu_pid)"
  fi
  if command -v nc >/dev/null 2>&1; then
    nc -z 127.0.0.1 "$GDB_PORT" >/dev/null 2>&1 && { ready=1; log_ready; break; }
  else
    (echo >/dev/tcp/127.0.0.1/"$GDB_PORT") >/dev/null 2>&1 && { ready=1; log_ready; break; }
  fi
  sleep 0.05
done
[[ "$ready" == "1" ]] || die "GDB port not ready: $GDB_PORT"

wait "$qemu_pid"
rm -f "$PIDFILE" || true
