#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - destroys the partition table and filesystems on the exact confirmed device
# open_questions: []
# ---

set -Eeuo pipefail
_part_script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
source "$_part_script_dir/common.sh"

create_partition_table() {
  local device="$1"
  myos2_run_root sgdisk --zap-all "$device"
  myos2_run_root sgdisk --clear "$device"
  myos2_run_root sgdisk -n 1:0:+256M -t 1:ef00 -c 1:"EFI System Partition" "$device"
  myos2_run_root sgdisk -n 2:0:0 -t 2:8300 -c 2:"MyOS2 rootfs" "$device"
  myos2_run_root partprobe "$device"
}

format_partitions() {
  local device="$1" efi root i
  efi="$(myos2_partition_path "$device" 1)"
  root="$(myos2_partition_path "$device" 2)"
  for ((i = 0; i < 50; i++)); do
    [[ -b "$efi" && -b "$root" ]] && break
    sleep 0.1
  done
  myos2_require_block_device "$efi"
  myos2_require_block_device "$root"
  myos2_run_root mkfs.fat -F 32 -n MYOS2_EFI "$efi"
  myos2_run_root mkfs.ext4 -F -L MYOS2_ROOT "$root"
}

part_vdisk() {
  local device="${1:-${MYOS2_TARGET_DISK:-}}"
  [[ -n "$device" ]] || myos2_die "set MYOS2_TARGET_DISK or pass a block device"
  device="$(myos2_canonical_path "$device")"
  myos2_require_cmd sgdisk partprobe mkfs.fat mkfs.ext4 lsblk
  myos2_require_block_device "$device"
  myos2_refuse_mounted_device "$device"
  myos2_confirm_destructive "$device"
  create_partition_table "$device"
  format_partitions "$device"
  myos2_info "partitioning complete: $device"
}

main() {
  case "${1:-}" in
    -h|--help)
      cat <<'EOF'
Usage: part_vdisk.sh [BLOCK_DEVICE]

Required safety gate:
  MYOS2_ALLOW_DESTRUCTIVE=1
  MYOS2_CONFIRM_TARGET=<same canonical block device>
EOF
      ;;
    "")
      part_vdisk "${MYOS2_TARGET_DISK:-}"
      ;;
    *)
      part_vdisk "$1"
      ;;
  esac
}

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
  main "$@"
fi
