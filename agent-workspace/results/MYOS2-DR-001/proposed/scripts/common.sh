#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - destructive helpers require two explicit confirmation variables
# open_questions: []
# ---

if [[ -n "${MYOS2_COMMON_SH_LOADED:-}" ]]; then
  return 0 2>/dev/null || exit 0
fi
readonly MYOS2_COMMON_SH_LOADED=1

myos2_info()  { printf 'MYOS2: %s\n' "$*" >&2; }
myos2_warn()  { printf 'MYOS2 WARNING: %s\n' "$*" >&2; }
myos2_die()   { printf 'MYOS2 ERROR: %s\n' "$*" >&2; exit 1; }

myos2_require_cmd() {
  local cmd
  for cmd in "$@"; do
    command -v "$cmd" >/dev/null 2>&1 || myos2_die "required command not found: $cmd"
  done
}

myos2_require_env() {
  local name="$1"
  [[ -n "${!name:-}" ]] || myos2_die "required environment variable is unset: $name"
}

myos2_canonical_path() {
  local path="$1"
  if command -v realpath >/dev/null 2>&1; then
    realpath -m -- "$path"
  else
    python3 - "$path" <<'PY'
import os, sys
print(os.path.abspath(sys.argv[1]))
PY
  fi
}

myos2_run_root() {
  if (( EUID == 0 )); then
    "$@"
  else
    myos2_require_cmd sudo
    sudo -- "$@"
  fi
}

myos2_require_regular_file() {
  local path="$1"
  [[ -f "$path" ]] || myos2_die "regular file not found: $path"
}

myos2_require_directory() {
  local path="$1"
  [[ -d "$path" ]] || myos2_die "directory not found: $path"
}

myos2_require_block_device() {
  local path="$1"
  [[ -b "$path" ]] || myos2_die "not a block device: $path"
}

myos2_require_mountpoint() {
  local path="$1"
  myos2_require_cmd mountpoint
  mountpoint -q -- "$path" || myos2_die "not an active mountpoint: $path"
}

myos2_refuse_mounted_device() {
  local device="$1"
  myos2_require_cmd lsblk
  if lsblk -nrpo MOUNTPOINT -- "$device" | grep -q '[^[:space:]]'; then
    myos2_die "device or one of its children is mounted: $device"
  fi
}

myos2_confirm_destructive() {
  local raw_target="$1"
  local target confirm
  target="$(myos2_canonical_path "$raw_target")"
  [[ "${MYOS2_ALLOW_DESTRUCTIVE:-0}" == "1" ]] ||
    myos2_die "refusing destructive action; set MYOS2_ALLOW_DESTRUCTIVE=1"
  [[ -n "${MYOS2_CONFIRM_TARGET:-}" ]] ||
    myos2_die "refusing destructive action; set MYOS2_CONFIRM_TARGET=$target"
  confirm="$(myos2_canonical_path "$MYOS2_CONFIRM_TARGET")"
  [[ "$confirm" == "$target" ]] ||
    myos2_die "confirmation mismatch: target=$target confirmation=$confirm"
  myos2_warn "destructive action authorized for exactly: $target"
}

myos2_install_with_optional_sudo() {
  local prefix="$1"
  shift
  if [[ -w "$prefix" ]]; then
    "$@"
  else
    myos2_run_root "$@"
  fi
}

myos2_partition_path() {
  local device="$1" number="$2"
  if [[ "$device" =~ [0-9]$ ]]; then
    printf '%sp%s\n' "$device" "$number"
  else
    printf '%s%s\n' "$device" "$number"
  fi
}
