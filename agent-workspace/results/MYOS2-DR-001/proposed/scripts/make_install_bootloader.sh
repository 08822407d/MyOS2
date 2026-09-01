#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - this proposal consumes an EFI artifact; it does not claim to reproduce the EDK2 build
# open_questions:
#   - choose the authoritative EFI build pipeline
# ---

_boot_script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
source "$_boot_script_dir/common.sh"
source "$_boot_script_dir/prepare_env.sh"

resolve_bootloader_efi() {
  local candidate="${MYOS2_BOOTLOADER_EFI:-}"
  if [[ -n "$candidate" ]]; then
    candidate="$(myos2_canonical_path "$candidate")"
  elif [[ -f "$MYOS2_ROOT_MYLOADER/BOOTX64.EFI" ]]; then
    candidate="$MYOS2_ROOT_MYLOADER/BOOTX64.EFI"
    myos2_warn "MYOS2_BOOTLOADER_EFI unset; using repository prebuilt $candidate"
  else
    myos2_die "set MYOS2_BOOTLOADER_EFI to a built BOOTX64.EFI"
  fi
  myos2_require_regular_file "$candidate"
  printf '%s\n' "$candidate"
}

build_bootloader() {
  local efi
  efi="$(resolve_bootloader_efi)"
  myos2_info "bootloader artifact selected: $efi"
  if command -v sha256sum >/dev/null 2>&1; then
    sha256sum -- "$efi" >&2
  fi
}

install_bootloader() {
  local prefix="${1:-${MYOS2_INSTALL_ROOT:-}}"
  local efi
  [[ -n "$prefix" ]] || myos2_die "install_bootloader requires a prefix or MYOS2_INSTALL_ROOT"
  efi="$(resolve_bootloader_efi)"
  myos2_run_root mkdir -p -- "$prefix/EFI/BOOT"
  myos2_run_root install -m 0644 -- "$efi" "$prefix/EFI/BOOT/BOOTX64.EFI"
}

# Compatibility aliases for current misspelled public function names.
build_boostloader() { build_bootloader "$@"; }
install_boostloader() { install_bootloader "$@"; }

unset _boot_script_dir
