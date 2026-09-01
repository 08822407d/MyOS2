#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# open_questions:
#   - canonical musl version/commit must be selected by the project
# ---

_initramfs_script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
source "$_initramfs_script_dir/common.sh"
source "$_initramfs_script_dir/prepare_env.sh"

: "${MYOS2_MUSL_PREFIX:=$MYOS2_DEPS_ROOT/musl-prefix}"

myos2_find_musl_cc() {
  local candidate
  for candidate in \
    "$MYOS2_MUSL_PREFIX/bin/musl-gcc" \
    "$MYOS2_MUSL_PREFIX/exec/bin/musl-gcc"; do
    [[ -x "$candidate" ]] && { printf '%s\n' "$candidate"; return 0; }
  done
  myos2_die "musl-gcc not found under $MYOS2_MUSL_PREFIX; set MYOS2_MUSL_PREFIX or run build_musl"
}

build_musl() {
  local source="${MYOS2_MUSL_SRC:-}"
  local build_dir="$MYOS2_DEPS_ROOT/musl-build"
  [[ -n "$source" ]] || myos2_die "set MYOS2_MUSL_SRC to a pinned musl source tree"
  source="$(myos2_canonical_path "$source")"
  myos2_require_regular_file "$source/configure"
  myos2_require_cmd make
  mkdir -p -- "$build_dir" "$MYOS2_MUSL_PREFIX"
  (
    cd -- "$build_dir"
    "$source/configure" --prefix="$MYOS2_MUSL_PREFIX" --target=x86_64
    make -j"$MYOS2_JOBS"
    make install
  )
}

build_initramfs() {
  local build_dir="$MYOS2_BUILD_ROOT/initramfs"
  local musl_cc
  musl_cc="$(myos2_find_musl_cc)"
  myos2_require_cmd cmake
  mkdir -p -- "$build_dir"
  cmake -S "$MYOS2_ROOT_MYINITRAMFS" -B "$build_dir" \
    -DCMAKE_BUILD_TYPE="$MYOS2_BUILD_TYPE" \
    -DCMAKE_C_COMPILER="$musl_cc" \
    -DMYOS2_MUSL_PREFIX="$MYOS2_MUSL_PREFIX"
  cmake --build "$build_dir" --parallel "$MYOS2_JOBS"
}

package_initramfs() {
  local build_dir="$MYOS2_BUILD_ROOT/initramfs"
  local stage_dir="$MYOS2_BUILD_ROOT/initramfs-stage"
  local output="$MYOS2_ARTIFACT_ROOT/myinitramfs.gz"
  myos2_require_cmd cmake cpio gzip
  [[ -d "$build_dir" ]] || myos2_die "run build_initramfs first"
  rm -rf -- "$stage_dir"
  mkdir -p -- "$stage_dir" "$MYOS2_ARTIFACT_ROOT"
  DESTDIR="$stage_dir" cmake --install "$build_dir" --prefix /
  (
    cd -- "$stage_dir"
    find . -print0 | cpio --null -o --format=newc | gzip -9 > "$output"
  )
  myos2_info "created $output"
}

install_initramfs() {
  local prefix="${1:-${MYOS2_INSTALL_ROOT:-}}"
  local artifact="$MYOS2_ARTIFACT_ROOT/myinitramfs.gz"
  [[ -n "$prefix" ]] || myos2_die "install_initramfs requires a prefix or MYOS2_INSTALL_ROOT"
  myos2_require_regular_file "$artifact"
  myos2_run_root mkdir -p -- "$prefix/boot"
  myos2_run_root cp -f -- "$artifact" "$prefix/boot/myinitramfs.gz"
}

unset _initramfs_script_dir
