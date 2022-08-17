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