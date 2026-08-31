#!/usr/bin/env bash
# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# open_questions: []
# ---

_prepare_script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"

if [[ -z "${MYOS2_ROOT:-}" ]]; then
  if command -v git >/dev/null 2>&1 &&
     _git_root="$(git -C "$_prepare_script_dir" rev-parse --show-toplevel 2>/dev/null)"; then
    MYOS2_ROOT="$_git_root"
    printf 'MYOS2 WARNING: MYOS2_ROOT unset; auto-detected %s\n' "$MYOS2_ROOT" >&2
  else
    _candidate="$(cd -- "$_prepare_script_dir/.." && pwd -P)"
    if [[ -d "$_candidate/mykernel" && -d "$_candidate/myloader" && -d "$_candidate/myinitramfs" ]]; then
      MYOS2_ROOT="$_candidate"
      printf 'MYOS2 WARNING: MYOS2_ROOT unset; inferred %s\n' "$MYOS2_ROOT" >&2
    else
      printf 'MYOS2 ERROR: set MYOS2_ROOT to the repository root\n' >&2
      return 1 2>/dev/null || exit 1
    fi
  fi
fi

MYOS2_ROOT="$(cd -- "$MYOS2_ROOT" && pwd -P)"
for _required in mykernel myloader myinitramfs; do
  [[ -d "$MYOS2_ROOT/$_required" ]] || {
    printf 'MYOS2 ERROR: %s is not a MyOS2 repository root\n' "$MYOS2_ROOT" >&2
    return 1 2>/dev/null || exit 1
  }
done

: "${MYOS2_BUILD_ROOT:=$MYOS2_ROOT/out/build}"
: "${MYOS2_ARTIFACT_ROOT:=$MYOS2_ROOT/out/artifacts}"
: "${MYOS2_DEPS_ROOT:=$MYOS2_ROOT/out/deps}"
: "${MYOS2_BUILD_TYPE:=Debug}"
if [[ -z "${MYOS2_JOBS:-}" ]]; then
  if command -v nproc >/dev/null 2>&1; then
    MYOS2_JOBS="$(nproc)"
  else
    MYOS2_JOBS=1
  fi
fi

export MYOS2_ROOT
export MYOS2_ROOT_MYKERNEL="$MYOS2_ROOT/mykernel"
export MYOS2_ROOT_MYLOADER="$MYOS2_ROOT/myloader"
export MYOS2_ROOT_MYINITRAMFS="$MYOS2_ROOT/myinitramfs"
export MYOS2_BUILD_ROOT MYOS2_ARTIFACT_ROOT MYOS2_DEPS_ROOT
export MYOS2_BUILD_TYPE MYOS2_JOBS

unset _prepare_script_dir _git_root _candidate _required
