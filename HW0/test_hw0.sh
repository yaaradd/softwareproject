#!/bin/bash

# Test script for hw0.c base converter
# Usage: ./test_hw0.sh

BC="./bc"
PASSED=0
FAILED=0

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test function
test_case() {
    local test_name="$1"
    local input="$2"
    local expected_output="$3"
    
    echo -n "Testing: $test_name... "
    
    # Run the program with input and capture output
    output=$(printf "$input" | $BC 2>&1)
    
    # Check if output contains expected string
    if echo "$output" | grep -q "$expected_output"; then
        echo -e "${GREEN}PASSED${NC}"
        ((PASSED++))
        return 0
    else
        echo -e "${RED}FAILED${NC}"
        echo "  Expected: $expected_output"
        echo "  Got: $output"
        ((FAILED++))
        return 1
    fi
}

# Test function for exact output match
test_exact() {
    local test_name="$1"
    local input="$2"
    local expected_output="$3"
    
    echo -n "Testing: $test_name... "
    
    output=$(printf "$input" | $BC 2>&1)
    
    if [ "$output" = "$expected_output" ]; then
        echo -e "${GREEN}PASSED${NC}"
        ((PASSED++))
        return 0
    else
        echo -e "${RED}FAILED${NC}"
        echo "  Expected:"
        echo "$expected_output" | sed 's/^/    /'
        echo "  Got:"
        echo "$output" | sed 's/^/    /'
        ((FAILED++))
        return 1
    fi
}

echo "=========================================="
echo "Testing Base Converter (hw0.c)"
echo "=========================================="
echo ""

# Test 1: Binary to Decimal (1010 = 10)
test_case "Binary to Decimal: 1010 -> 10" \
    "2\n10\n1010\n" \
    "The number in base 10 is:"

# Test 2: Decimal to Binary (10 = 1010)
test_case "Decimal to Binary: 10 -> 1010" \
    "10\n2\n10\n" \
    "The number in base 2 is:"

# Test 3: Decimal to Hexadecimal (255 = ff)
test_case "Decimal to Hex: 255 -> ff" \
    "10\n16\n255\n" \
    "The number in base 16 is:"

# Test 4: Hexadecimal to Decimal (ff = 255)
test_case "Hex to Decimal: ff -> 255" \
    "16\n10\nff\n" \
    "The number in base 10 is:"

# Test 5: Binary to Hexadecimal (1010 = a)
test_case "Binary to Hex: 1010 -> a" \
    "2\n16\n1010\n" \
    "The number in base 16 is:"

# Test 6: Zero in any base
test_case "Zero conversion: 0 in base 10 to base 2" \
    "10\n2\n0\n" \
    "0"

# Test 7: Single digit conversion
test_case "Single digit: 5 in base 10 to base 2" \
    "10\n2\n5\n" \
    "The number in base 2 is:"

# Test 8: Octal to Decimal (777 = 511)
test_case "Octal to Decimal: 777 -> 511" \
    "8\n10\n777\n" \
    "The number in base 10 is:"

# Test 9: Invalid source base (too low)
test_case "Invalid source base: 1" \
    "1\n" \
    "Invalid source base!"

# Test 10: Invalid source base (too high)
test_case "Invalid source base: 17" \
    "17\n" \
    "Invalid source base!"

# Test 11: Invalid target base (too low)
test_case "Invalid target base: 1" \
    "10\n1\n" \
    "Invalid target base!"

# Test 12: Invalid target base (too high)
test_case "Invalid target base: 17" \
    "10\n17\n" \
    "Invalid target base!"

# Test 13: Invalid number (digit too high for base)
test_case "Invalid number: 8 in base 8" \
    "8\n10\n8\n" \
    "Invalid input number!"

# Test 14: Invalid number (invalid character)
test_case "Invalid number: 'g' in base 16" \
    "16\n10\ng\n" \
    "Invalid input number!"

# Test 15: Invalid number (uppercase in base 16 - should fail if not supported)
test_case "Invalid number: 'G' in base 16" \
    "16\n10\nG\n" \
    "Invalid input number!"

# Test 16: Base 3 conversion
test_case "Base 3 to Decimal: 210 -> 21" \
    "3\n10\n210\n" \
    "The number in base 10 is:"

# Test 17: Base 5 conversion
test_case "Base 5 to Decimal: 4321 -> 586" \
    "5\n10\n4321\n" \
    "The number in base 10 is:"

# Test 18: Large number conversion
test_case "Large number: 12345 in base 10 to base 16" \
    "10\n16\n12345\n" \
    "The number in base 16 is:"

# Test 19: Hex with letters
test_case "Hex with letters: abc in base 16 to base 10" \
    "16\n10\nabc\n" \
    "The number in base 10 is:"

# Test 20: Base 2 to Base 8
test_case "Binary to Octal: 1010 -> 12" \
    "2\n8\n1010\n" \
    "The number in base 8 is:"

# Test 21: Maximum valid base (16)
test_case "Maximum base: base 16 to base 16" \
    "16\n16\nf\n" \
    "The number in base 16 is:"

# Test 22: Minimum valid base (2)
test_case "Minimum base: base 2 to base 2" \
    "2\n2\n1\n" \
    "The number in base 2 is:"

# Test 23: Same base conversion (identity)
test_case "Same base: 123 in base 10 to base 10" \
    "10\n10\n123\n" \
    "The number in base 10 is:"

# Test 24: Invalid source base (0)
test_case "Invalid source base: 0" \
    "0\n" \
    "Invalid source base!"

# Test 25: Invalid source base (negative)
test_case "Invalid source base: -5" \
    "-5\n" \
    "Invalid source base!"

# Test 26: Invalid target base (0)
test_case "Invalid target base: 0" \
    "10\n0\n" \
    "Invalid target base!"

# Test 27: Invalid target base (negative)
test_case "Invalid target base: -10" \
    "10\n-10\n" \
    "Invalid target base!"

# Test 28: All zeros in different base
test_case "Multiple zeros: 0000 in base 2" \
    "2\n10\n0000\n" \
    "The number in base 10 is:"

# Test 29: Boundary digit in base 2 (only 0 and 1 valid)
test_case "Invalid digit 2 in base 2" \
    "2\n10\n102\n" \
    "Invalid input number!"

# Test 30: Boundary digit in base 10 (9 is max)
test_case "Valid digit 9 in base 10" \
    "10\n16\n9\n" \
    "The number in base 16 is:"

# Test 31: Invalid digit in base 3 (3 is invalid, max is 2)
test_case "Invalid digit 3 in base 3" \
    "3\n10\n3\n" \
    "Invalid input number!"

# Test 32: Letter 'a' in base 10 (invalid)
test_case "Invalid letter 'a' in base 10" \
    "10\n16\na\n" \
    "Invalid input number!"

# Test 33: Letter 'f' in base 15 (invalid, max is 'e')
test_case "Invalid letter 'f' in base 15" \
    "15\n10\nf\n" \
    "Invalid input number!"

# Test 34: Valid 'e' in base 15
test_case "Valid letter 'e' in base 15" \
    "15\n10\ne\n" \
    "The number in base 10 is:"

# Test 35: Maximum hex digit 'f' in base 16
test_case "Maximum hex digit 'f' in base 16" \
    "16\n10\nf\n" \
    "The number in base 10 is:"

# Test 36: Mixed valid hex number
test_case "Mixed hex: 1a2b3c in base 16" \
    "16\n10\n1a2b3c\n" \
    "The number in base 10 is:"

# Test 37: Base 11 with digit 'a' (10 in base 11)
test_case "Base 11 with 'a': a in base 11 to base 10" \
    "11\n10\na\n" \
    "The number in base 10 is:"

# Test 38: Invalid 'b' in base 11 (only up to 'a' valid)
test_case "Invalid 'b' in base 11" \
    "11\n10\nb\n" \
    "Invalid input number!"

# Test 39: Base 4 to base 16 conversion
test_case "Base 4 to hex: 3210 in base 4" \
    "4\n16\n3210\n" \
    "The number in base 16 is:"

# Test 40: Single '0' conversion
test_case "Single zero: 0 in base 16 to base 2" \
    "16\n2\n0\n" \
    "0"

# Test 41: Leading valid digits in valid base
test_case "Leading 1s: 1111 in base 2" \
    "2\n10\n1111\n" \
    "The number in base 10 is:"

# Test 42: All 'f's in hex
test_case "All f's: fff in base 16" \
    "16\n10\nfff\n" \
    "The number in base 10 is:"

# Test 43: Base 7 conversion (mid-range base)
test_case "Base 7: 654 in base 7 to base 10" \
    "7\n10\n654\n" \
    "The number in base 10 is:"

# Test 44: Invalid digit '7' in base 7
test_case "Invalid digit 7 in base 7" \
    "7\n10\n7\n" \
    "Invalid input number!"

# Test 45: Base 13 with valid 'c'
test_case "Base 13 with 'c': c in base 13" \
    "13\n10\nc\n" \
    "The number in base 10 is:"

# Test 46: Invalid 'd' in base 13 (max is 'c')
test_case "Invalid 'd' in base 13" \
    "13\n10\nd\n" \
    "Invalid input number!"

# Test 47: Power of 2 in binary
test_case "Power of 2: 10000 in base 2 (16 in decimal)" \
    "2\n10\n10000\n" \
    "The number in base 10 is:"

# Test 48: Number with all valid letters for base 16
test_case "All letters hex: abcdef in base 16" \
    "16\n10\nabcdef\n" \
    "The number in base 10 is:"

# Test 49: Special character (should be invalid)
test_case "Invalid character: @ in base 16" \
    "16\n10\n@\n" \
    "Invalid input number!"

# Test 50: Space in number (should be invalid if not handled)
test_case "Number with space: '1 0' in base 10" \
    "10\n16\n1 0\n" \
    "Invalid input number!"

# Test 51: Hyphen/dash (should be invalid)
test_case "Invalid character: -5 in base 10" \
    "10\n16\n-5\n" \
    "Invalid input number!"

# Test 52: Plus sign (should be invalid)
test_case "Invalid character: +5 in base 10" \
    "10\n16\n+5\n" \
    "Invalid input number!"

# Test 53: Base 6 boundary test
test_case "Base 6 max digit: 5 in base 6" \
    "6\n10\n5\n" \
    "The number in base 10 is:"

# Test 54: Invalid 6 in base 6
test_case "Invalid digit 6 in base 6" \
    "6\n10\n6\n" \
    "Invalid input number!"

# Test 55: Base 9 conversion
test_case "Base 9 to decimal: 876 in base 9" \
    "9\n10\n876\n" \
    "The number in base 10 is:"

# Test 56: Alternating binary pattern
test_case "Alternating binary: 10101010 in base 2" \
    "2\n16\n10101010\n" \
    "The number in base 16 is:"

# Test 57: Octal to binary conversion
test_case "Octal to binary: 7 in base 8 to base 2" \
    "8\n2\n7\n" \
    "The number in base 2 is:"

# Test 58: Large hex number
test_case "Large hex: fedcba in base 16" \
    "16\n10\nfedcba\n" \
    "The number in base 10 is:"

# Test 59: Base 12 with 'b'
test_case "Base 12 with 'b': b9a in base 12" \
    "12\n10\nb9a\n" \
    "The number in base 10 is:"

# Test 60: Binary all 1s (power of 2 minus 1)
test_case "Binary all 1s: 11111111 in base 2" \
    "2\n10\n11111111\n" \
    "The number in base 10 is:"


echo ""
echo "=========================================="
echo "Test Results:"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo "=========================================="

if [ $FAILED -eq 0 ]; then
    exit 0
else
    exit 1
fi

