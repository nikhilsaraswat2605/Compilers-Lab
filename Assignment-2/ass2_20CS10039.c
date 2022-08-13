#include "myl.h"

int printStr(char * str){
    int len = 0;                
    while(str[len] != '\0')     // continue till the null character is found
        len++;
    __asm__ __volatile__(       // print the string
        "movl $1, %%eax\n\t"    // eax = 1
        "movq $1, %%rdi\n\t"    // rdi = 1
        "syscall\n\t"           // system call
        :
        :"S"(str), "d"(len)     // pass the string and the length of the string to the system call
    );
    return len;                 // return the length of the string
}

int readInt(int *n){

}


int printInt(int n){
    char buff[32];              // buffer to store the string
    int len=0,neg=0;            // length of the string, flag to check if the number is negative
    if(n<0){                    // if the number is negative
        buff[len++]='-';        // add the negative sign to the string
        n=-n;                   // make the number positive
        neg=1;                  // set the flag
    }
    while(n){                   // continue till the number is not zero
        buff[len++]=n%10+'0';   // add unit digit to the string
        n/=10;                  // remove the unit digit
    }
    int i=0;                    // index to traverse the string
    if(neg)                     // if the number is negative
        i++;                    // increment the index
    while(i<len/2){             // reverse the string
        char temp=buff[i];
        buff[i]=buff[len-i-1];
        buff[len-i-1]=temp;
        i++;
    }
    buff[len]='\0';             // add the null character to the string
    printStr(buff);             // print the string
    return len;                 // return the length of the string
}

int readFlt(float *f){

}

int printFlt(float f){
    long long int int_part = (long long int)f;  // get the integer part of the number
    long long int frac_part = (long long int)((f - (float)int_part)*1000000);       // get the fractional part of the number with 6 decimal places
    int numchar = printInt(int_part);           // print the integer part of the number
    numchar += printStr(".");                   // print the decimal point
    if(frac_part == 0)                          // if the fractional part is zero
        numchar += printStr("0");               // print the fractional part as zero
    else                                        
        numchar += printInt(frac_part);         // print the fractional part
    return numchar;                             // return the number of characters printed
}


