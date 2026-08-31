#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - partition and sync-physical remain fail-closed destructive commands
# open_questions: []
# ---

set -Eeuo pipefail
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
source "$SCRIPT_DIR/scripts/common.sh"
source "$SCRIPT_DIR/scripts/prepare_env.sh"
source "$SCRIPT_DIR/scripts/make_install_kernel.sh"
source "$SCRIPT_DIR/scripts/make_install_initranfs.sh"
source "$SCRIPT_DIR/scripts/make_install_bootloader.sh"

usage() {
  cat <<'EOF'
Usage:
  make_install.sh doctor
  make_install.sh build kernel|initramfs|bootloader|all
  make_install.sh package initramfs|all
  make_install.sh install kernel|initramfs|bootloader|all PREFIX
  make_install.sh qemu [--gdb-wait|--no-gdb-wait]
  make_install.sh partition [BLOCK_DEVICE]
  make_install.sh sync-physical [SOURCE_MOUNT DEST_MOUNT]
  make_install.sh help
EOF
}

doctor() {
  local failed=0 cmd
  for cmd in bash cmake python3; do
    if command -v "$cmd" >/dev/null 2>&1; then
      printf 'ok      %s: %s\n' "$cmd" "$(command -v "$cmd")"
    else
      printf 'missing %s\n' "$cmd"
      failed=1
    fi
  done
  printf 'root    %s\n' "$MYOS2_ROOT"
  printf 'build   %s\n' "$MYOS2_BUILD_ROOT"
  printf 'artifact %s\n' "$MYOS2_ARTIFACT_ROOT"
  return "$failed"
}

build_component() {
  case "$1" in
    kernel) build_kernel ;;
    initramfs) build_initramfs ;;
    bootloader) build_bootloader ;;
    all) build_kernel; build_initramfs; build_bootloader ;;
    *) myos2_die "unknown build component: $1" ;;
  esac
}

package_component() {
  case "$1" in
    initramfs|all) package_initramfs ;;
    *) myos2_die "unknown package component: $1" ;;
  esac
}

install_component() {
  local component="$1" prefix="$2"
  case "$component" in
    kernel) install_kernel "$prefix" ;;
    initramfs) install_initramfs "$prefix" ;;
    bootloader) install_bootloader "$prefix" ;;
    all)
      install_kernel "$prefix"
      install_initramfs "$prefix"
      install_bootloader "$prefix"
      ;;
    *) myos2_die "unknown install component: $component" ;;
  esac
}

command_name="${1:-help}"
case "$command_name" in
  help|-h|--help) usage ;;
  doctor) doctor ;;
  build)
    [[ $# -eq 2 ]] || myos2_die "usage: make_install.sh build COMPONENT"
    build_component "$2"
    ;;
  package)
    [[ $# -eq 2 ]] || myos2_die "usage: make_install.sh package COMPONENT"
    package_component "$2"
    ;;
  install)
    [[ $# -eq 3 ]] || myos2_die "usage: make_install.sh install COMPONENT PREFIX"
    install_component "$2" "$3"
    ;;
  qemu)
    shift
    exec "$SCRIPT_DIR/dbg-qemu.sh" "$@"
    ;;
  partition)
    shift
    exec "$SCRIPT_DIR/scripts/part_vdisk.sh" "$@"
    ;;
  sync-physical)
    shift
    exec "$SCRIPT_DIR/scripts/phys_nvme_install.sh" "$@"
    ;;
  *) usage >&2; myos2_die "unknown command: $command_name" ;;
esac
