#!/usr/bin/env bash
# base_snapshot: time@a039d9803893e0ce54164413aa8a0c29eacf5c78
#
# Generic bounded QEMU/headless smoke-test harness for MYOS2-DR-006.
#
# The script intentionally does NOT hard-code a MyOS2 image path or QEMU
# invocation that has not been frozen by this research task.
#
# Usage:
#
#   run-qemu-smoke.sh \
#       --timeout-seconds 45 \
#       --success-regex '^MYOS2_BOOT_OK$' \
#       --fatal-regex '^MYOS2_(FATAL|PANIC)' \
#       --log qemu-serial.log \
#       -- qemu-system-i386 ... -nographic ...
#
# Success:
#   success marker is observed before fatal/timeout/early process exit.
#
# Failure:
#   fatal marker, timeout, or process exit before the success marker.

set -euo pipefail

timeout_seconds=45
success_regex='^MYOS2_BOOT_OK$'
fatal_regex='^MYOS2_(FATAL|PANIC)'
log_path='qemu-smoke.log'
poll_seconds='0.20'

usage() {
    cat <<'EOF'
Usage:
  run-qemu-smoke.sh [options] -- <qemu command> [args...]

Options:
  --timeout-seconds N   Hard deadline in seconds (default: 45)
  --success-regex RX    Extended-regex success oracle
  --fatal-regex RX      Extended-regex fatal oracle
  --log PATH            Complete guest/QEMU output log
  -h, --help            Show this help
EOF
}

die() {
    printf 'MYOS2_HARNESS_ERROR %s\n' "$*" >&2
    exit 2
}

while (($#)); do
    case "$1" in
        --timeout-seconds)
            (($# >= 2)) || die "--timeout-seconds requires a value"
            timeout_seconds="$2"
            shift 2
            ;;
        --success-regex)
            (($# >= 2)) || die "--success-regex requires a value"
            success_regex="$2"
            shift 2
            ;;
        --fatal-regex)
            (($# >= 2)) || die "--fatal-regex requires a value"
            fatal_regex="$2"
            shift 2
            ;;
        --log)
            (($# >= 2)) || die "--log requires a value"
            log_path="$2"
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        --)
            shift
            break
            ;;
        *)
            die "unknown option: $1"
            ;;
    esac
done

(($# > 0)) || die "missing QEMU command after --"

[[ "$timeout_seconds" =~ ^[1-9][0-9]*$ ]] \
    || die "--timeout-seconds must be a positive integer"

mkdir -p "$(dirname "$log_path")"
: >"$log_path"

qemu_pid=''

cleanup() {
    if [[ -n "${qemu_pid:-}" ]] && kill -0 "$qemu_pid" 2>/dev/null; then
        kill "$qemu_pid" 2>/dev/null || true

        # Give QEMU a brief opportunity to leave cleanly.
        for _ in 1 2 3 4 5; do
            if ! kill -0 "$qemu_pid" 2>/dev/null; then
                break
            fi
            sleep 0.1
        done

        if kill -0 "$qemu_pid" 2>/dev/null; then
            kill -KILL "$qemu_pid" 2>/dev/null || true
        fi

        wait "$qemu_pid" 2>/dev/null || true
    fi
}

trap cleanup EXIT INT TERM

printf 'MYOS2_HARNESS_START timeout=%s log=%s\n' \
    "$timeout_seconds" "$log_path"

printf 'MYOS2_HARNESS_COMMAND'
printf ' %q' "$@"
printf '\n'

# Redirect the complete QEMU stream to a persistent log.  CI may print it on
# failure and/or retain it as an artifact.
"$@" >"$log_path" 2>&1 &
qemu_pid=$!

start_seconds=$SECONDS

while true; do
    # Fatal wins if it appears before the success condition is accepted.
    if grep -Eq "$fatal_regex" "$log_path"; then
        printf 'MYOS2_HARNESS_FAIL reason=fatal-marker\n' >&2
        cat "$log_path" >&2
        exit 1
    fi

    if grep -Eq "$success_regex" "$log_path"; then
        printf 'MYOS2_HARNESS_PASS reason=success-marker\n'
        cat "$log_path"
        exit 0
    fi

    if ! kill -0 "$qemu_pid" 2>/dev/null; then
        set +e
        wait "$qemu_pid"
        qemu_rc=$?
        set -e

        qemu_pid=''

        # Close a tiny race where the final bytes were written immediately
        # before process exit.
        if grep -Eq "$fatal_regex" "$log_path"; then
            printf 'MYOS2_HARNESS_FAIL reason=fatal-marker qemu_rc=%s\n' \
                "$qemu_rc" >&2
        elif grep -Eq "$success_regex" "$log_path"; then
            printf 'MYOS2_HARNESS_PASS reason=success-marker qemu_rc=%s\n' \
                "$qemu_rc"
            cat "$log_path"
            exit 0
        else
            printf 'MYOS2_HARNESS_FAIL reason=early-qemu-exit qemu_rc=%s\n' \
                "$qemu_rc" >&2
        fi

        cat "$log_path" >&2
        exit 1
    fi

    elapsed=$((SECONDS - start_seconds))

    if ((elapsed >= timeout_seconds)); then
        printf 'MYOS2_HARNESS_FAIL reason=timeout elapsed=%s\n' \
            "$elapsed" >&2
        cat "$log_path" >&2
        exit 124
    fi

    sleep "$poll_seconds"
done
