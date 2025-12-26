#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int charToDigit(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}

char digitToChar(int digit) {
    if (digit >= 0 && digit <= 9) {
        return '0' + digit;
    } else if (digit >= 10 && digit <= 15) {
        return 'a' + (digit - 10);
    }
    return '?';
}

int isValidBase(int base) {
    return base >= 2 && base <= 16;
}

int isValidNumber(long number, int base) {
    long temp;
    int digit;
    
    if (number == 0) {
        return 1;
    }
    
    temp = number;
    while (temp > 0) {
        digit = temp % 100;
        if (digit >= base) {
            return 0;
        }
        temp = temp / 100;
    }
    return 1;
}

long readNumber(int base) {
    int c;
    long number = 0;
    int valid = 1;
    
    while ((c = getchar()) != '\n' && c != EOF) {
        int digit = charToDigit((char)c);
        if (digit == -1 || digit >= base) {
            valid = 0;
        }
        number = number * 100 + digit;
    }
    
    if (!valid) {
        return -1;
    }
    return number;
}

long convertToDecimal(long number, int sourceBase) {
    long decimal = 0;
    long power = 1;
    
    while (number > 0) {
        int digit = number % 100;
        decimal = decimal + digit * power;
        power = power * sourceBase;
        number = number / 100;
    }
    
    return decimal;
}

void convertFromDecimal(long decimal, int targetBase) {
    long temp;
    long reversed = 0;
    int digitCount = 0;
    
    if (decimal == 0) {
        printf("0\n");
        return;
    }
    
    temp = decimal;
    while (temp > 0) {
        int digit = temp % targetBase;
        reversed = reversed * 100 + digit;
        temp = temp / targetBase;
        digitCount++;
    }
    
    while (digitCount > 0) {
        int digit = reversed % 100;
        printf("%c", digitToChar(digit));
        reversed = reversed / 100;
        digitCount--;
    }
    printf("\n");
}

int main() {
    int sourceBase, targetBase;
    long number;
    long decimal;
    
    printf("enter the source base:\n");
    scanf("%d", &sourceBase);
    getchar();
    
    if (!isValidBase(sourceBase)) {
        printf("Invalid source base!\n");
        return 0;
    }
    
    printf("enter the target base:\n");
    scanf("%d", &targetBase);
    getchar();
    
    if (!isValidBase(targetBase)) {
        printf("Invalid target base!\n");
        return 0;
    }
    
    printf("enter a number in base %d:\n", sourceBase);
    number = readNumber(sourceBase);
    
    if (number == -1) {
        printf("Invalid input number!\n");
        return 0;
    }
    
    decimal = convertToDecimal(number, sourceBase);
    
    printf("The number in base %d is:\n", targetBase);
    convertFromDecimal(decimal, targetBase);
    
    return 0;
}
