#include "myl.h"

int printStr(char * str){
    int len = 0;
    while(str[len] != '\0')     // continue till the null character is found
        len++;
    __asm__ __volatile__(
        "movl $1, %%eax\n\t"
        "movq $1, %%rdi\n\t"
        "syscall\n\t"
        :
        :"S"(str), "d"(len)
    );
}

int readInt(int *n){
    
}


int printInt(int n){

}

int readFlt(float *f){

}

int printFlt(float f){

}


