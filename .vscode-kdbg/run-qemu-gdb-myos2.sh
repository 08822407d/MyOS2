#!/usr/bin/env bash
set -euo pipefail

QEMU_BIN="${QEMU_BIN:-qemu-system-x86_64}"
GDB_PORT="${GDB_PORT:-8864}"
PIDFILE="${PIDFILE:-$PWD/.qemu-myos2.pid}"

# 0/1：是否启用 KVM
USE_KVM="${USE_KVM:-1}"

# --- UEFI/OVMF：默认在工程根目录 myloader/ 下，文件名用 edk2 默认名 ---
# 允许外部传入覆盖：OVMF_DIR=/path/to/FV_or_myloader
OVMF_DIR="${OVMF_DIR:-$PWD/myloader}"
OVMF_CODE="${OVMF_CODE:-$OVMF_DIR/OVMF_CODE.fd}"
OVMF_VARS_TEMPLATE="${OVMF_VARS_TEMPLATE:-$OVMF_DIR/OVMF_VARS.fd}"

# 可写 NVRAM（VARS）输出文件（建议每种模式各一份）
OVMF_VARS_OUT="${OVMF_VARS_OUT:-$PWD/.vscode-kdbg/ovmf-vars-myos2.fd}"

# --- 你的系统是“硬盘 + UEFI bootloader”启动 ---
DISK_IMG="${DISK_IMG:-$HOME/vmware/myos2test/myos2test-flat.vmdk}"
DISK_IF="${DISK_IF:-virtio}"    # 你若没做 virtio 驱动，可改为 ide/ahci
DISK_FORMAT="${DISK_FORMAT:-}"  # 不填则自动判断（flat->raw，descriptor->vmdk）

# 机器与显示（你需要 PS/2 键盘）
MACHINE="${MACHINE:-pc-q35-noble,i8042=on}"
DISPLAY_OPTS="${DISPLAY_OPTS:--display gtk}"

MEM_MB="${MEM_MB:-8192}"
SMP_OPTS="${SMP_OPTS:-1}"

# CPU：KVM 用 host；TCG 用 max（你已验证 max,hypervisor=off 可用）
if [[ -z "${CPU_OPTS:-}" ]]; then
  if [[ "$USE_KVM" == "1" ]]; then
    CPU_OPTS="host,hypervisor=off,kvm=off"
  else
    CPU_OPTS="max,hypervisor=off"
  fi
fi

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

# --- OVMF 校验与 VARS 复制 ---
[[ -f "$OVMF_CODE" ]] || die "OVMF_CODE not found: $OVMF_CODE"
[[ -f "$OVMF_VARS_TEMPLATE" ]] || die "OVMF_VARS template not found: $OVMF_VARS_TEMPLATE"

mkdir -p "$(dirname "$OVMF_VARS_OUT")"
if [[ ! -f "$OVMF_VARS_OUT" ]]; then
  cp -f "$OVMF_VARS_TEMPLATE" "$OVMF_VARS_OUT"
fi

UEFI_ARGS=(
  -drive "if=pflash,format=raw,readonly=on,file=${OVMF_CODE}"
  -drive "if=pflash,format=raw,file=${OVMF_VARS_OUT}"
)

# --- 磁盘参数 ---
[[ -e "$DISK_IMG" ]] || die "Disk image not found: $DISK_IMG"

if [[ -z "${DISK_FORMAT:-}" ]]; then
  # 自动判断：flat.* 当 raw；descriptor .vmdk 当 vmdk
  if [[ "$DISK_IMG" == *"-flat."* || "$DISK_IMG" == *"-flat"* ]]; then
    DISK_FORMAT="raw"
  else
    DISK_FORMAT="vmdk"
  fi
fi

DISK_ARGS=(-drive "file=${DISK_IMG},if=${DISK_IF},format=${DISK_FORMAT}")

# --- KVM 开关 ---
KVM_ARGS=()
if [[ "$USE_KVM" == "1" ]]; then
  KVM_ARGS=(-enable-kvm)
fi

log_start

"$QEMU_BIN" \
  "${KVM_ARGS[@]}" \
  -machine "$MACHINE" \
  -m "$MEM_MB" \
  -smp "$SMP_OPTS" \
  -cpu "$CPU_OPTS" \
  "${UEFI_ARGS[@]}" \
  -gdb "tcp::${GDB_PORT},server,nowait" \
  -S \
  "${DISK_ARGS[@]}" \
  -net none \
  -serial mon:stdio \
  $DISPLAY_OPTS \
  &

qemu_pid=$!
echo "$qemu_pid" > "$PIDFILE"

# wait gdb port ready (<= ~10s)
for _ in $(seq 1 200); do
  if ! kill -0 "$qemu_pid" 2>/dev/null; then
    die "QEMU exited early (pid=$qemu_pid)"
  fi
  if command -v nc >/dev/null 2>&1; then
    nc -z 127.0.0.1 "$GDB_PORT" >/dev/null 2>&1 && { log_ready; break; }
  else
    (echo >/dev/tcp/127.0.0.1/"$GDB_PORT") >/dev/null 2>&1 && { log_ready; break; }
  fi
  sleep 0.05
done

wait "$qemu_pid"
rm -f "$PIDFILE" || true