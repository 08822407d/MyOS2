#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - mapping a writable image changes it; callers own mount and cleanup
# open_questions: []
# ---

_map_script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
# shellcheck source=common.sh
source "$_map_script_dir/common.sh"

myos2_find_free_nbd() {
  local sys_name dev
  myos2_run_root modprobe nbd max_part=16
  for dev in /dev/nbd*; do
    [[ "$dev" =~ ^/dev/nbd[0-9]+$ ]] || continue
    [[ -b "$dev" ]] || continue
    sys_name="${dev##*/}"
    if [[ ! -s "/sys/class/block/$sys_name/pid" ]]; then
      printf '%s\n' "$dev"
      return 0
    fi
  done
  myos2_die "no free /dev/nbd device; set MYOS2_NBD_DEVICE after freeing one"
}

map_vdisk() {
  local image="${1:-${MYOS2_VDISK:-}}"
  local device format
  [[ -n "$image" ]] || myos2_die "usage: map_vdisk <image>; or set MYOS2_VDISK"
  image="$(myos2_canonical_path "$image")"
  myos2_require_regular_file "$image"
  myos2_require_cmd qemu-nbd partprobe

  if [[ -n "${MYOS2_NBD_DEVICE:-}" ]]; then
    device="$MYOS2_NBD_DEVICE"
    myos2_require_block_device "$device"
    [[ ! -s "/sys/class/block/${device##*/}/pid" ]] ||
      myos2_die "requested NBD is already connected: $device"
  else
    device="$(myos2_find_free_nbd)"
    myos2_warn "MYOS2_NBD_DEVICE unset; selected free device $device"
  fi

  if [[ -n "${MYOS2_VDISK_FORMAT:-}" ]]; then
    format="$MYOS2_VDISK_FORMAT"
  else
    case "${image##*.}" in
      vmdk|VMDK) format=vmdk ;;
      qcow2|QCOW2) format=qcow2 ;;
      img|raw|RAW) format=raw ;;
      *) myos2_die "set MYOS2_VDISK_FORMAT for image: $image" ;;
    esac
    myos2_warn "MYOS2_VDISK_FORMAT unset; inferred '$format' from filename"
  fi

  myos2_run_root qemu-nbd --connect="$device" --format="$format" "$image"
  if ! myos2_run_root partprobe "$device"; then
    myos2_run_root qemu-nbd --disconnect "$device" || true
    myos2_die "partition table probe failed for $device"
  fi
  printf '%s\n' "$device"
}

unmap_vdisk() {
  local device="$1"
  myos2_require_block_device "$device"
  myos2_require_cmd qemu-nbd
  myos2_run_root qemu-nbd --disconnect "$device"
}

# Compatibility name. Unlike the old implementation, this function never mounts.
map_vdisk_once() {
  map_vdisk "$@"
}

unset _map_script_dir
