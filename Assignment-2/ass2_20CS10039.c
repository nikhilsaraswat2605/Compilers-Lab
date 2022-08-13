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
    char buff[32];
    int len=0,neg=0;
    if(n<0){
        buff[len++]='-';
        n=-n;
        neg=1;
    }
    while(n){
        buff[len++]=n%10+'0';
        n/=10;
    }
    int i=0;
    if(neg)
        i++;
    while(i<len/2){
        char temp=buff[i];
        buff[i]=buff[len-i-1];
        buff[len-i-1]=temp;
        i++;
    }
    buff[len]='\0';
    printStr(buff);
    return len;
}

int readFlt(float *f){

}

int printFlt(float f){
    long long int int_part = (long long int)f;
    long long int frac_part = (long long int)((f - (float)int_part)*1000000);
    printInt(int_part);
    printStr(".");
    if(frac_part == 0)
        printStr("0");
    else
        printInt(frac_part);
}


