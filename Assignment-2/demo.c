#include "myl.h"
#define BUFF_SIZE 100

// Function to print a string (character array), and return the number of characters printed
int printStr(char *str)
{
    int len = 0;
    while(str[len] != '\0')     // continue till the null character is found
        len++;

    __asm__ __volatile__ (
        "movl $1, %%eax \n\t"
        "movq $1, %%rdi \n\t"
        "syscall \n\t"
        :
        :"S"(str), "d"(len)
    );
    return len;
}


// Function to read an integer from STDIN
int readInt(int *n)
{
    // ranges for positive and negative integers
    const long int MAX_INT_POSITIVE = 2147483647;
    const long int MAX_INT_NEGATIVE = 2147483648;

    char buff[BUFF_SIZE];
    int len, i = 0, isNegative = 0;
    long int num = 0;

    __asm__ __volatile__ (
        "movl $0, %%eax \n\t"
        "movq $0, %%rdi \n\t"
        "syscall \n\t"
        :"=a"(len)
        :"S"(buff), "d"(BUFF_SIZE)
    );

    if(len <= 0)
        return ERR;
    if((buff[0] != '+') && (buff[0] != '-') && (buff[0] < '0' || buff[0] > '9'))    // check for valid first character
        return ERR;

    if(buff[0] == '-' || buff[0] == '+') {      // check for sign if present
        if(buff[0] == '-')
            isNegative = 1;
        i++;
        if(buff[i] < '0' || buff[i] > '9')
            return ERR;
    }

    // continue till a space, tab or newline is found
    while(buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\t') {
        if(buff[i] < '0' || buff[i] > '9')      // invalid character
            return ERR;
        int digit = buff[i] - '0';

        // check for range
        if(!isNegative && 1L * num * 10 + digit > MAX_INT_POSITIVE)
            return ERR;
        else if(isNegative && 1L * num * 10 + digit > MAX_INT_NEGATIVE)
            return ERR;
        
        num = num * 10 + digit;
        i++;
    }

    if(isNegative)      // if the original number was negative, multiply by -1
        num *= -1;
    *n = (int)num;
    return OK;
}


// Function to print an integer, and return the number of characters printed
int printInt(int n)
{   
    char buff[BUFF_SIZE];
    int i = 0, j, k;
    int sign = (n < 0 ? -1 : 1);
    if(n == 0)
        buff[i++] = '0';        // handle n = 0 separately
    else {
        if(n < 0)
            buff[i++] = '-';

        while(n) {
            buff[i++] = '0' + ((n % 10) * sign);
            n /= 10;
        }

        // the number has been placed in the array in reverse order, so we need to reverse it
        j = (buff[0] == '-' ? 1 : 0);
        k = i - 1;
        for(; j < k; j++, k--) {
            char temp = buff[j];
            buff[j] = buff[k];
            buff[k] = temp;
        }
    }
    int size = i;

    __asm__ __volatile__ (
        "movl $1, %%eax \n\t"
        "movq $1, %%rdi \n\t"
        "syscall \n\t"
        :
        :"S"(buff), "d"(size)
    );
    return size;
}


// Function to read a float from STDIN
// Capable of reading floats like 12.65, -0.859, .26, 1.5e6, -0.25E-2, etc.
int readFlt(float *f)
{
    char buff[BUFF_SIZE];
    int len, i = 0, j, sign = 1, expFound = 0, dotFound = 0, expSign = 1, exp = 0;
    float num = 0, div = 1, expVal = 1;

    __asm__ __volatile__ (
        "movl $0, %%eax \n\t"
        "movq $0, %%rdi \n\t"
        "syscall \n\t"
        :"=a"(len)
        :"S"(buff), "d"(BUFF_SIZE)
    );
    if(len <= 0)
        return ERR;
    if((buff[0] != '+') && (buff[0] != '-') && (buff[0] != '.') && (buff[0] < '0' || buff[0] > '9'))    // check for valid first character
        return ERR;

    if(buff[i] == '-' || buff[i] == '+') {      // check for sign if present
        if(buff[0] == '-')
            sign = -1;
        i++;
        if((buff[i] < '0' || buff[i] > '9') && buff[i] != '.')
            return ERR;
    }

    // process the part before 'e' or 'E' if present
    // we process the number as if decimal point is not present, and then divide it by 10^(no. of digits after decimal point)
    while(buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\t') {
        if(buff[i] == 'e' || buff[i] == 'E') {      // break if 'e' or 'E' is present
            expFound = 1;
            break;
        }
        else if(buff[i] == '.') {       // decimal point found
            if(dotFound)        // decimal point present twice, so return ERR
                return ERR;
            else 
                dotFound = 1;
        }
        else if(buff[i] >= '0' && buff[i] <= '9') {
            int digit = buff[i] - '0';
            num = num * 10.0 + (float)digit;
            if(dotFound)
                div *= (float)10.0;
        }
        else    // invalid character
            return ERR;
        i++;
    }

    num *= sign;
    num /= div;

    // process the exponent part if 'e' or 'E' if present
    if(expFound) {
        i++;
        if((buff[i] != '+') && (buff[i] != '-') && (buff[i] < '0' || buff[i] > '9'))
            return ERR;
        if(buff[i] == '-' || buff[i] == '+') {
            if(buff[i] == '-')
                expSign = -1;
            i++;
            if(buff[i] < '0' || buff[i] > '9')
                return ERR;
        }

        // continue till a space, tab or newline is found
        while(buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\t') {
            if(buff[i] < '0' || buff[i] > '9')      // invalid character
                return ERR;
            else {
                int digit = buff[i] - '0';
                exp = exp * 10 + digit;
            }
            i++;
        }
        
        // multiply or divide by the exponent as necessary
        for(j = 0; j < exp; j++)
            expVal *= 10.0;
        if(expSign > 0)
            num *= expVal;
        else
            num /= expVal;
    }   

    *f = num;
    return OK;
}


// Function to print a float, and return the number of characters printed
int printFlt(float f)
{
    char buff[BUFF_SIZE];
    int i = 0, j, k;
    if(f < 0) {
        f *= -1.0;
        buff[i++] = '-';
    }
    long long int integralPart = (long long int)f;      // extract the integral part
    long long int fractionalPart = (long long int)((f - integralPart) * 1000000);   // extract the fcational part, we need upto 6 decimal places

    j = i;
    if(integralPart == 0) {
        buff[i++] = '0';
    }
    else {      // process the integral part
        while(integralPart > 0) {
            buff[i++] = '0' + (integralPart % 10);
            integralPart /= 10;
        }

        // this part has to be reversed, as it is stored in reverse order in the array
        k = i - 1;
        for(; j < k; j++, k--) {
            char temp = buff[j];
            buff[j] = buff[k];
            buff[k] = temp;
        }
    }

    buff[i++] = '.';
    i = i + 5;      // since we are considering upto 6 decimal places
    int size = i + 1;
    int iter = 0;
    while(iter < 6) {       // process the fractional part
        buff[i--] = '0' + (fractionalPart % 10);
        fractionalPart /= 10;
        iter++;
    }

    __asm__ __volatile__ (
        "movl $1, %%eax \n\t"
        "movq $1, %%rdi \n\t"
        "syscall \n\t"
        :
        :"S"(buff), "d"(size)
    );
    return size;
}