#include "myl.h"

#ifndef BUFF_SIZE
#define BUFF_SIZE 32
#endif

int readInt(int *n)
{
    char buffer[BUFF_SIZE] = {};
    int len = 0, isNeg = 0;
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
    if(len==2 && (buffer[0]=='+' || buffer[0]=='-'))
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
        isNeg = buffer[0] == '-' ? 1 : 0; // if the first character is '-', set isNeg to 1, else set isNeg to 0
        i = 1;                            // set i to 1
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
