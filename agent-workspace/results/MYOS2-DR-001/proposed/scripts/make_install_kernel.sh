#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# open_questions: []
# ---

_kernel_script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
source "$_kernel_script_dir/common.sh"
source "$_kernel_script_dir/prepare_env.sh"
source "$_kernel_script_dir/map_vdisk.sh"

build_kernel() {
  local build_dir="$MYOS2_BUILD_ROOT/kernel"
  myos2_require_cmd cmake
  mkdir -p -- "$build_dir"
  cmake -S "$MYOS2_ROOT_MYKERNEL" -B "$build_dir" \
    -DCMAKE_BUILD_TYPE="$MYOS2_BUILD_TYPE" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
  cmake --build "$build_dir" --parallel "$MYOS2_JOBS"
}

install_kernel() {
  local prefix="${1:-${MYOS2_INSTALL_ROOT:-}}"
  local build_dir="$MYOS2_BUILD_ROOT/kernel"
  [[ -n "$prefix" ]] || myos2_die "install_kernel requires a prefix or MYOS2_INSTALL_ROOT"
  mkdir -p -- "$prefix"
  [[ -d "$build_dir" ]] || myos2_die "kernel build directory absent; run build_kernel first"
  myos2_install_with_optional_sudo "$prefix" \
    cmake --install "$build_dir" --prefix "$prefix"
}

make_kernel_vmdk_bootable() (
  set -Eeuo pipefail
  local image="${1:-${MYOS2_VDISK:-}}"
  local device="" mount_dir=""
  [[ -n "$image" ]] || myos2_die "pass image path or set MYOS2_VDISK"
  cleanup() {
    if [[ -n "$mount_dir" ]] && mountpoint -q -- "$mount_dir" 2>/dev/null; then
      myos2_run_root umount -- "$mount_dir" || true
    fi
    [[ -z "$device" ]] || unmap_vdisk "$device" || true
    [[ -z "$mount_dir" ]] || rmdir -- "$mount_dir" 2>/dev/null || true
  }
  trap cleanup EXIT

  build_kernel
  device="$(map_vdisk "$image")"
  mount_dir="$(mktemp -d "${TMPDIR:-/tmp}/myos2-kernel.XXXXXX")"
  myos2_run_root mount -- "$(myos2_partition_path "$device" 2)" "$mount_dir"
  install_kernel "$mount_dir"
)

unset _kernel_script_dir
