/******************************************************************************
Name: NIKHIL SARASWAT
Roll No: 20CS10039
Assignment: 2
*******************************************************************************/
#include "myl.h"
#include <stdio.h>

#define BUFF_SIZE 32 // buffer size
#ifndef INT_MAX
#define INT_MAX __INT32_MAX__ // maximum int value
#endif
#ifndef INT_MIN
#define INT_MIN (-INT_MAX - 1) // minimum int value
#endif

// In this program, I have used some functions again, to make code easy to understand and for 're-usability'.

int printStr(char *str) // function to print string
{
    int len = 0; // length of string
    while (str[len] != '\0')
    { // loop to find length of string
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

int printInt(int n) // function to print integer
{
    char buffer[BUFF_SIZE] = {}; // buffer to store the string
    int len = 0, isNeg = 0;      // length of the string, flag to check if the number is isNegative
    long long int temp_num = n;  // temp_num is the absolute value of n
    if (temp_num == 0)           //  if number is 0, store char '0' in buffer
    {
        buffer[len++] = '0';
    }
    if (temp_num < 0) // if the number is negative
    {
        temp_num = -temp_num; // make the number positive
        isNeg = 1;            // set the flag
    }
    while (temp_num > 0)                     // if the number is positive
    {                                        // continue till the number is not zero
        buffer[len++] = temp_num % 10 + '0'; // add unit digit to the string
        temp_num /= 10;                      // remove the unit digit
    }
    if (isNeg) // if the number is negative, i.e. isNeg flag is 1
    {
        buffer[len++] = '-'; // if the number is Negative, add the Negative sign to the string
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

int printFlt(float f) //  function to print float
{
    long double temp_num = f;                                                                                   // convert the float to a long double for better precision
    int isNeg = 0;                                                                                              // flag to check if the number is negative
    long long int integer_part = (long long int)temp_num;                                                       // get the integer part of the number
    long long int fractional_part = (long long int)((temp_num - (long double)integer_part) * 1000000000000000); // get the fractional part of the number with 6 decimal places
    long double temp_frac = (temp_num - (long double)integer_part);
    // printf("%Lf\n", temp_frac);
    int zeros = 0; // number of zeros to be printed
    while(temp_frac > 0)
    {
        temp_frac *= 10;
        if(temp_frac < 1)
        {
            zeros++;
        }else break;
    }
    // printf("%d\n", zeros);
    if (temp_num < 0)
    { // if the number is isNegative
        isNeg = 1;
        fractional_part = -fractional_part; // make the fractional part positive
    }
    while (fractional_part > 0)
    { // if the fractional part is isNegative, make it positive
        if (fractional_part % 10 == 0 || fractional_part >= 100000000)
        {                          // if the fractional part is divisible by 10
            fractional_part /= 10; // divide by 10
        }
        else if (fractional_part < 100000000)
        { // if the fractional part is not divisible by 10
            break;
        }
    }
    if (isNeg && integer_part == 0)
        printStr("-");
    int numchar = printInt(integer_part); // print the integer part of the number
    numchar += printStr(".");             // print the decimal point
    while(zeros--)
    {
        numchar += printStr("0"); // print the zeros
    }
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

int readInt(int *n)
{
    char buffer[BUFF_SIZE] = {};
    int len = 0, isisNeg = 0;
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
    if ((buffer[0] < '0' || buffer[0] > '9') && buffer[0] != '-' && buffer[0] != '+')
    { // if the first character of the string is not a digit, return 0
        return ERR;
    }
    if (len == 2 && (buffer[0] == '+' || buffer[0] == '-'))
    {
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
    if (buffer[0] == '-' || buffer[0] == '+')
    {
        isisNeg = buffer[0] == '-' ? 1 : 0; // if the first character is '-', set isisNeg to 1, else set isisNeg to 0
        i = 1;                              // set i to 1
    }
    long long int temp_num = 0;
    temp_num = 0; // set n to 0
    for (; i < len - 1; i++)
    {
        temp_num = temp_num * 10 + (buffer[i] - '0'); // convert the string to an integer
        if (temp_num > INT_MAX)
            return ERR;
        if (isisNeg && temp_num > (long long int)INT_MAX + 1LL)
            return ERR;
    }
    if (isisNeg)
    { // if the string is isNegative, make it positive
        temp_num = -(temp_num);
    }
    *n = temp_num; // set n to the integer
    return OK;     // return OK
}

int readFlt(float *temp_num)
{
    char buffer[BUFF_SIZE] = {};      // buffer to store the string
    int len = 0, isisNeg = 0;         // length of the string, flag to check if the number is isNegative
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
    if ((buffer[0] < '0' || buffer[0] > '9') && buffer[0] != '-' && buffer[0] != '.' && buffer[0] != '+')
    { // if the first character of the string is not a digit, return 0
        return ERR;
    }
    if (len == 2 && (buffer[0] == '+' || buffer[0] == '-' || buffer[0] == '.'))
    {
        return ERR;
    }
    int count_decimal_chars = 0; // count the number of decimal characters
    if (buffer[0] == '.')
    {
        count_decimal_chars++;
    }
    int digits = 0;
    for (int i = 1; i < len - 1; i++)
    {
        if (buffer[i] == '.')
        { // if the character is '.', increment the count
            count_decimal_chars++;
        }
        if (((buffer[i] < '0' || buffer[i] > '9') && buffer[i] != '.') || count_decimal_chars > 1)
        { // if the character is not a digit or '.' or number of decimal characters > 0, return ERR
            return ERR;
        }
        else if (buffer[i] >= '0' && buffer[i] <= '9') // if the character is a digit, increment the digits
            digits++;
    }
    if (digits == 0) // if the number of digits is 0, return ERR
    {
        return ERR;
    }

    int i = 0; // index to traverse the string
    if (buffer[0] == '-' || buffer[0] == '+')
    {
        isisNeg = buffer[0] == '-' ? 1 : 0; // if the first character is '-', set isisNeg to 1, else set isisNeg to 0
        i = 1;                              // set i to 1
    }
    double integer_part = 0; // integer part of the number
    for (; i < len - 1; i++)
    {
        if (buffer[i] == '.')
        { // if the character is '.', break
            break;
        }
        integer_part = integer_part * 10 + (buffer[i] - '0'); // add digit to number
        if (integer_part > INT_MAX)                           // if the number is greater than INT_MAX, return ERR
            return ERR;
        if (isisNeg && integer_part > (long long int)INT_MAX + 1LL) // if the number is Negative and greater than INT_MAX + 1, i.e. actual number < INT_MIN, return ERR
            return ERR;
    }
    double fractional_part = 0; // decimal part of the number
    for (int j = len - 2; j > i; j--)
    {
        fractional_part = fractional_part + (buffer[j] - '0'); // add digit to number
        fractional_part = fractional_part / 10;                // divide by 10
    }
    *temp_num = integer_part + fractional_part; // add the integer and decimal parts
    if (isisNeg)
    { // if the number is Negative, make it positive
        *temp_num = -(*temp_num);
    }
    return OK; // return OK
}
