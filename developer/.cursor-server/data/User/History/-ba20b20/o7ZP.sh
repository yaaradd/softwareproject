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

