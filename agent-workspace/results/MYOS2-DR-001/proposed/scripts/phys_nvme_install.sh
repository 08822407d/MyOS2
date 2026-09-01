#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - clears the top level of the exact confirmed destination mountpoint
# open_questions:
#   - should future versions sync the whole rootfs instead of the legacy directory set?
# ---

set -Eeuo pipefail
_phys_script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
source "$_phys_script_dir/common.sh"

sync_physical_root() {
  local source_root="${1:-${MYOS2_SOURCE_ROOT:-}}"
  local dest_root="${2:-${MYOS2_DEST_ROOT:-}}"
  local name
  local -a legacy_entries=(bin etc usr sbin mylib boot efi)

  [[ -n "$source_root" ]] || myos2_die "set MYOS2_SOURCE_ROOT or pass source mountpoint"
  [[ -n "$dest_root" ]] || myos2_die "set MYOS2_DEST_ROOT or pass destination mountpoint"
  source_root="$(myos2_canonical_path "$source_root")"
  dest_root="$(myos2_canonical_path "$dest_root")"

  [[ "$source_root" != "$dest_root" ]] || myos2_die "source and destination are identical"
  [[ "$dest_root" != "/" ]] || myos2_die "refusing destination /"
  myos2_require_mountpoint "$source_root"
  myos2_require_mountpoint "$dest_root"
  myos2_confirm_destructive "$dest_root"
  myos2_require_cmd find cp sync

  myos2_warn "clearing top-level entries under $dest_root"
  myos2_run_root find "$dest_root" -mindepth 1 -maxdepth 1 -exec rm -rf -- {} +

  for name in "${legacy_entries[@]}"; do
    if [[ -e "$source_root/$name" ]]; then
      myos2_run_root cp -a -- "$source_root/$name" "$dest_root/"
    else
      myos2_warn "legacy source entry absent, skipped: $source_root/$name"
    fi
  done
  sync
  myos2_info "physical root sync complete: $dest_root"
}

main() {
  case "${1:-}" in
    -h|--help)
      cat <<'EOF'
Usage: phys_nvme_install.sh [SOURCE_MOUNT DEST_MOUNT]

Both paths must already be mountpoints. The destination is cleared only after:
  MYOS2_ALLOW_DESTRUCTIVE=1
  MYOS2_CONFIRM_TARGET=<same canonical destination mountpoint>
EOF
      ;;
    *)
      sync_physical_root "${1:-}" "${2:-}"
      ;;
  esac
}

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
  main "$@"
fi
