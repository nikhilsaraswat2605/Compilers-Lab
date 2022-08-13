#include "myl.h"

#define BUFF_SIZE 32

int printStr(char * str){
    int len = 0;                                    
    while(str[len] != '\0')                                                 // continue till the null character is found
        len++;
    __asm__ __volatile__(                                                   // print the string
        "movl $1, %%eax\n\t"                                                // eax = 1
        "movq $1, %%rdi\n\t"                                                // rdi = 1
        "syscall\n\t"                                                       // system call
        :
        :"S"(str), "d"(len)                                                 // pass the string and the length of the string to the system call
    );
    return len;                                                             // return the length of the string
}

int readInt(int *n){
    char buf[BUFF_SIZE];
    int len, isNeg = 0;
    __asm__ __volatile__(                                                   // read the string
        "movl $0, %%eax\n\t"                                                // eax = 0
        "movq $0, %%rdi\n\t"                                                // rdi = 0
        "syscall\n\t"                                                       // system call
         :"=a"(len)                                                         // return the length of the string
        :"S"(buf), "d"(BUFF_SIZE)                                           // pass the buffer and the length of the buffer to the system call
    );
    // printInt(len);                                                       // print the string
    if(len <= 0)                                                            // if the length of the string <=> 0, return 0
        return ERR;
    if(buf[len-1] == '\n')                                                  // if the last character of the string is '\n', remove it
        buf[len-1] = '\0';
    if((buf[0] < '0' || buf[0] > '9') && buf[0] != '-')                     // if the first character of the string is not a digit, return 0
        return ERR;
    for(int i = 1; i < len-1; i++){
        if(buf[i] < '0' || buf[i] > '9'){                                   // if the character is not a digit, return ERR
            return ERR;
        }
    }

    int i = 0;                                                              
    if(buf[0] == '-'){                                                      // if the first character of the string is '-', return 0
        isNeg = 1;                                                          // set isNeg to 1
        i = 1;                                                              // set i to 1
    }               
    *n = 0;                                                                 // set n to 0
    for(; i < len - 1; i++){
        *n = *n * 10 + (buf[i] - '0');                                      // convert the string to an integer
    }
    if(isNeg)                                                               // if the string is negative, make it positive
        *n = -(*n);
    return OK;                                                              // return OK
}


int printInt(int n){
    char buff[BUFF_SIZE];                                                   // buffer to store the string
    int len=0,neg=0;                                                        // length of the string, flag to check if the number is negative
    if(n == 0){
        buff[len++] = '0';
    }
    if(n<0){                                                                // if the number is negative
        buff[len++]='-';                                                    // add the negative sign to the string
        n=-n;                                                               // make the number positive
        neg=1;                                                              // set the flag
    }
    while(n){                                                               // continue till the number is not zero
        buff[len++]=n%10+'0';                                               // add unit digit to the string
        
        n/=10;                                                              // remove the unit digit
    }
    buff[len]='\0';                                                         // add the null character to the string
    int i=0;                                                                // index to traverse the string
    if(neg)                                                                 // if the number is negative
        i++,len++;                                                          // increment the index

    while(i<len/2){                                                         // reverse the string
        char temp=buff[i];
        buff[i]=buff[len-i-1];
        buff[len-i-1]=temp;
        i++;
    }
    printStr(buff);                                                         // print the string
    return len;                                                             // return the length of the string
}

int readFlt(float *f){
    char buf[BUFF_SIZE];                                                    // buffer to store the string
    int len, isNeg = 0;                                                     // length of the string, flag to check if the number is negative
    __asm__ __volatile__(                                                   // read the string
        "movl $0, %%eax\n\t"                                                // eax = 0
        "movq $0, %%rdi\n\t"                                                // rdi = 0
        "syscall\n\t"                                                       // system call
         :"=a"(len)                                                         // return the length of the string
        :"S"(buf), "d"(BUFF_SIZE)                                           // pass the buffer and the length of the buffer to the system call
    );
    // printInt(len);                                                       // print the string
    if(len <= 0)                                                            // if the length of the string <= 0, return 0
        return ERR;
    if(buf[len-1] == '\n')                                                  // if the last character of the string is '\n', remove it
        buf[len-1] = '\0';
    if((buf[0] < '0' || buf[0] > '9') && buf[0] != '-')                     // if the first character of the string is not a digit, return 0
        return ERR;
    int count_decimal_chars=0;                                              // count the number of decimal characters
    for(int i = 1; i < len-1; i++){
        if(buf[i]=='.'){                                                    // if the character is '.', increment the count
            count_decimal_chars++;
        }
        if((buf[i] < '0' || buf[i] > '9') && buf[i]!='.' && count_decimal_chars>1){      // if the character is not a digit or number of decimal characters > 1, return ERR
            return ERR;
        }
    }

    int i = 0;                                                              // index to traverse the string
    if(buf[0] == '-'){                                                      // if the first character of the string is '-', return 0
        isNeg = 1;
        i = 1;                                                              // increment the index
    }
    float integer_part=0;                                                   // integer part of the number
    for(; i < len - 1; i++){
        if(buf[i]=='.')                                                     // if the character is '.', break
            break;
        integer_part = integer_part * 10 + (buf[i] - '0');                  // add digit to number
    }
    float decimal_part=0;                                                   // decimal part of the number
    for(int j=len-2; j>i; j--){
        decimal_part = decimal_part + (buf[j] - '0');                       // add digit to number
        decimal_part = decimal_part / 10;                                   // divide by 10
    }
    *f=integer_part+decimal_part;                                           // add the integer and decimal parts
    if(isNeg)                                                               // if the number is negative, make it positive
        *f = -(*f);
    return OK;                                                              // return OK
}

int printFlt(float f){
    long long int int_part = (long long int)f;                                      // get the integer part of the number
    long long int frac_part = (long long int)((f - (float)int_part)*1000000);       // get the fractional part of the number with 6 decimal places
    if(f<0){                                                                        // if the number is negative
        frac_part = -frac_part;                                                     // make the fractional part positive
    }
    int numchar = printInt(int_part);                                               // print the integer part of the number
    numchar += printStr(".");                                                       // print the decimal point
    if(frac_part == 0)                                                              // if the fractional part is zero
        numchar += printStr("0");                                                   // print the fractional part as zero
    else                                        
        numchar += printInt(frac_part);                                             // print the fractional part
    return numchar;                                                                 // return the number of characters printed
}


