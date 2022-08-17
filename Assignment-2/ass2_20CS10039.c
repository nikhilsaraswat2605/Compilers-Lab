#include "myl.h"

#define BUFF_SIZE 32

int printStr(char *str)
{
    int len = 0;
    while (str[len] != '\0')
    { // continue till the null character is found
        len++;
    }
    __asm__ __volatile__(    // print the string
        "movl $1, %%eax\n\t" // eax = 1
        "movq $1, %%rdi\n\t" // rdi = 1
        "syscall\n\t"        // system call
        :
        : "S"(str), "d"(len) // pass the string and the length of the string to the system call
    );
    return len; // return the length of the string
}

int readInt(int *n)
{
    char buffer[BUFF_SIZE];
    int len, isNeg = 0;
    __asm__ __volatile__(             // read the string
        "movl $0, %%eax\n\t"          // eax = 0
        "movq $0, %%rdi\n\t"          // rdi = 0
        "syscall\n\t"                 // system call
        : "=a"(len)                   // return the length of the string
        : "S"(buffer), "d"(BUFF_SIZE) // pass the buffer and the length of the buffer to the system call
    );
    if (len <= 0)
    { // if the length of the string <=> 0, return 0
        return ERR;
    }
    if (buffer[len - 1] == '\n')
    { // if the last character of the string is '\n', remove it
        buffer[len - 1] = '\0';
    }
    if ((buffer[0] < '0' || buffer[0] > '9') && buffer[0] != '-')
    { // if the first character of the string is not a digit, return 0
        return ERR;
    }
    for (int i = 1; i < len - 1; i++)
    {
        if (buffer[i] < '0' || buffer[i] > '9')
        { // if the character is not a digit, return ERR
            return ERR;
        }
    }

    int i = 0;
    if (buffer[0] == '-')
    {              // if the first character of the string is '-', return 0
        isNeg = 1; // set isNeg to 1
        i = 1;     // set i to 1
    }
    long long int temp_num = 0;
    temp_num = 0; // set n to 0
    for (; i < len - 1; i++)
    {
        temp_num = temp_num * 10 + (buffer[i] - '0'); // convert the string to an integer
    }
    if (isNeg)
    { // if the string is negative, make it positive
        temp_num = -(temp_num);
    }
    *n = temp_num; // set n to the integer
    return OK;     // return OK
}

int printInt(int n)
{
    char buffer[BUFF_SIZE];     // buffer to store the string
    int len = 0, neg = 0;       // length of the string, flag to check if the number is negative
    long long int temp_num = n; // temp_num is the absolute value of n
    if (temp_num == 0)
    {
        buffer[len++] = '0';
    }
    if (temp_num < 0)
    {                         // if the number is negative
        temp_num = -temp_num; // make the number positive
        neg = 1;              // set the flag
    }
    while (temp_num > 0)
    {                                        // continue till the number is not zero
        buffer[len++] = temp_num % 10 + '0'; // add unit digit to the string
        temp_num /= 10;                      // remove the unit digit
    }
    if (neg)
    {
        buffer[len++] = '-'; // if the number is negative, add the negative sign to the string
    }
    buffer[len] = '\0'; // add the null character to the string
    int i = 0;          // index to traverse the string
    while (i < len / 2)
    { // reverse the string
        char temp = buffer[i];
        buffer[i] = buffer[len - i - 1];
        buffer[len - i - 1] = temp;
        i++;
    }
    printStr(buffer); // print the string
    return len;       // return the length of the string
}

int readFlt(float *f)
{
    char buffer[BUFF_SIZE];           // buffer to store the string
    int len, isNeg = 0;               // length of the string, flag to check if the number is negative
    __asm__ __volatile__(             // read the string
        "movl $0, %%eax\n\t"          // eax = 0
        "movq $0, %%rdi\n\t"          // rdi = 0
        "syscall\n\t"                 // system call
        : "=a"(len)                   // return the length of the string
        : "S"(buffer), "d"(BUFF_SIZE) // pass the buffer and the length of the buffer to the system call
    );
    // printInt(len);                                                       // print the string
    if (len <= 0)
    { // if the length of the string <= 0, return 0
        return ERR;
    }
    if (buffer[len - 1] == '\n')
    { // if the last character of the string is '\n', remove it
        buffer[len - 1] = '\0';
    }
    if ((buffer[0] < '0' || buffer[0] > '9') && buffer[0] != '-' && buffer[0] != '.')
    { // if the first character of the string is not a digit, return 0
        return ERR;
    }
    int count_decimal_chars = 0; // count the number of decimal characters
    for (int i = 1; i < len - 1; i++)
    {
        if (buffer[i] == '.')
        { // if the character is '.', increment the count
            count_decimal_chars++;
        }
        if ((buffer[i] < '0' || buffer[i] > '9') && buffer[i] != '.' && count_decimal_chars > 1)
        { // if the character is not a digit or number of decimal characters > 1, return ERR
            return ERR;
        }
    }

    int i = 0; // index to traverse the string
    if (buffer[0] == '-')
    { // if the first character of the string is '-', return 0
        isNeg = 1;
        i = 1; // increment the index
    }
    double integer_part = 0; // integer part of the number
    for (; i < len - 1; i++)
    {
        if (buffer[i] == '.')
        { // if the character is '.', break
            break;
        }
        integer_part = integer_part * 10 + (buffer[i] - '0'); // add digit to number
    }
    double fractional_part = 0; // decimal part of the number
    for (int j = len - 2; j > i; j--)
    {
        fractional_part = fractional_part + (buffer[j] - '0'); // add digit to number
        fractional_part = fractional_part / 10;                // divide by 10
    }
    *f = integer_part + fractional_part; // add the integer and decimal parts
    if (isNeg)
    { // if the number is negative, make it positive
        *f = -(*f);
    }
    return OK; // return OK
}

int printFlt(float f)
{
    long long int integer_part = (long long int)f;                                           // get the integer part of the number
    long long int fractional_part = (long long int)((f - (float)integer_part) * 1000000000); // get the fractional part of the number with 6 decimal places
    if (f < 0)
    {                                       // if the number is negative
        fractional_part = -fractional_part; // make the fractional part positive
    }
    while (fractional_part > 0)
    { // if the fractional part is negative, make it positive
        if (fractional_part % 10 == 0 || fractional_part >= 10000000)
        {                          // if the fractional part is divisible by 10
            fractional_part /= 10; // divide by 10
        }
        else if (fractional_part < 10000000)
        { // if the fractional part is not divisible by 10
            break;
        }
    }
    int numchar = printInt(integer_part); // print the integer part of the number
    numchar += printStr(".");             // print the decimal point
    if (fractional_part == 0)
    {                             // if the fractional part is zero
        numchar += printStr("0"); // print the fractional part as zero
    }
    else
    {
        numchar += printInt(fractional_part); // print the fractional part
    }
    return numchar; // return the number of characters printed
}
