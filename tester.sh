#!/bin/bash

FT_PING="./ft_ping"
SYS_PING=$(command -v ping)
TIMEOUT_DURATION=2
TOLERANCE_MS=30
OUT_DIR="output"

rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"

test_cases=(
    "8.8.8.8"
    "google.com"
    "localhost"
    "127.0.0.1"
    "192.0.2.123"
    "notarealhost.tld"
    "999.999.999.999"
    "invalid..address"
    "-v 8.8.8.8"
    "-v google.com"
    "-v 192.0.2.123"
    "-v notarealhost.tld"
    "-?"
    "-h"
    "--help"
	"-2"
)

# invalid or unsupported options
test_cases+=(
    "-x"
    "-vv"
    "-vh"
    "-hv"
    "--v"
    "--verbose"
    "-help"
    "-?"
    "-H"
)

# destination edge cases
test_cases+=(
    "0.0.0.0"
    "255.255.255.255"
    "127.1"
    "127"
    "1.1.1"
    "1.1.1.1.1"
    "256.1.1.1"
    "01.02.03.004"
    "8.8.8"
    ".8.8.8.8"
    "8.8.8.8."
)

# dns edge cases
test_cases+=(
    "localhost."
    "localhost.."
    ".localhost"
    "google..com"
    "google"
    "a"
    "a."
    "example.invalid"
    "example.test"
)

SELECTED_TEST=""
if [[ $# -gt 0 ]]; then
    if [[ "$1" =~ ^[0-9]+$ ]]; then
        SELECTED_TEST="$1"
    else
        echo "Usage: $0 [test_number]"
        echo "Example: $0 2"
        exit 1
    fi
fi

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
GRAY='\033[0;90m'
NC='\033[0m'

fail_idx=1

extract_time_ms() {
    echo "$1" | grep -m1 -o 'time=[0-9.]*' | head -n1 | cut -d= -f2
}

abs_diff_ms() {
    awk -v a="$1" -v b="$2" 'BEGIN{d=a-b; print (d<0?-d:d)}'
}

write_fail_log() {
    local kind="$1"
    local args="$2"
    local ft_exit="$3"
    local sys_exit="$4"
    local ft_out="$5"
    local sys_out="$6"
    local has_code_diff="$7"  # YES/NO
    local has_out_diff="$8"   # YES/NO

    local file="${OUT_DIR}/diff${fail_idx}"
    : > "$file"

    {
        echo "=============================="
        echo "${kind}: $args"
        echo "Date: $(date)"
        echo ""

	
        if [[ "$has_code_diff" == "YES" ]]; then
            echo "Exit code difference:"
            echo "  ft_ping: $ft_exit"
            echo "  system:  $sys_exit"
            echo ""
        fi

        if [[ "$has_out_diff" == "YES" ]]; then
            echo "Output difference:"
            echo ""
            echo "--- ft_ping output ---"
            echo "$ft_out"
            echo ""
            echo "--- system ping output ---"
            echo "$sys_out"
            echo ""
        fi

        echo "=============================="
        echo ""
    } >> "$file"

    echo -e "${GRAY}[CHECK DIFF → ${file}]${NC}"
    fail_idx=$((fail_idx + 1))
}

run_test() {
    local args="$1"

    echo -e "${YELLOW}=== Testing: ${NC}$args ${YELLOW}===${NC}"

    local ft_out sys_out ft_exit sys_exit
    ft_out=$(sudo timeout -s SIGINT $TIMEOUT_DURATION $FT_PING $args 2>&1)
    ft_exit=$?
    sys_out=$(timeout -s SIGINT $TIMEOUT_DURATION $SYS_PING $args 2>&1)
    sys_exit=$?

    # RTT handling (on-screen only)
    local ft_time sys_time diff time_pass="N/A"
    ft_time=$(extract_time_ms "$ft_out")
    sys_time=$(extract_time_ms "$sys_out")

    if [[ -n "$ft_time" && -n "$sys_time" ]]; then
        diff=$(abs_diff_ms "$ft_time" "$sys_time")
        if awk -v d="$diff" -v tol="$TOLERANCE_MS" 'BEGIN{exit(d>tol)}'; then
            echo -e "${GREEN}[RTT PASS]${NC} ft=${ft_time}ms sys=${sys_time}ms diff=${diff}ms"
            time_pass="PASS"
        else
            echo -e "${RED}[RTT FAIL]${NC} ft=${ft_time}ms sys=${sys_time}ms diff=${diff}ms"
            time_pass="FAIL"
        fi
    else
        echo -e "${GRAY}[RTT N/A]${NC} (missing time in one or both outputs)"
    fi

    # Compare outputs and exit codes
    local out_diff="NO" code_diff="NO"
    [[ "$ft_out" != "$sys_out" ]] && out_diff="YES"
    [[ "$ft_exit" != "$sys_exit" ]] && code_diff="YES"

	if [[ "$ft_exit" != "$sys_exit" ]]; then
		echo -e "${RED}[EXIT CODE FAIL] ft_ping=${ft_exit}, sys_ping=${sys_exit} ${NC}"
	else
		echo -e "${GREEN}[EXIT CODE PASS] ft_ping=${ft_exit}, sys_ping=${sys_exit} ${NC}"
	fi

    # Log only if something differs; RTT differences are not stored
    if [[ "$out_diff" == "YES" ]]; then
        write_fail_log "Test case" "$args" \
            "$ft_exit" "$sys_exit" \
            "$ft_out" "$sys_out" \
            "$code_diff" "$out_diff"
    fi

    echo -e "${BLUE}==============================${NC}"
    echo
}


if [[ -n "$SELECTED_TEST" ]]; then
    idx=$((SELECTED_TEST - 1))

    if (( idx < 0 || idx >= ${#test_cases[@]} )); then
        echo -e "${RED}Invalid test number.${NC}"
        echo "Valid range: 1 to ${#test_cases[@]}"
        exit 1
    fi

    echo -e "${BLUE}Running single test #$SELECTED_TEST:${NC} ${test_cases[$idx]}"
    echo
    run_test "${test_cases[$idx]}"
else
    for case in "${test_cases[@]}"; do
        run_test "$case"
    done
fi
