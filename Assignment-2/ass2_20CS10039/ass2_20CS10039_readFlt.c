#include "myl.h"

#ifndef BUFF_SIZE
#define BUFF_SIZE 32
#endif

int readFlt(float *f)
{
    char buffer[BUFF_SIZE] = {};      // buffer to store the string
    int len = 0, isNeg = 0;           // length of the string, flag to check if the number is negative
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
    if(len==2 && (buffer[0]=='+' || buffer[0]=='-'))
    {
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
    if (buffer[0] == '-' || buffer[0] == '+')
    {
        isNeg = buffer[0] == '-' ? 1 : 0; // if the first character is '-', set isNeg to 1, else set isNeg to 0
        i = 1;                            // set i to 1
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
