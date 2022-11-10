#include "myl.h"

#define AFTERDECIMAL 6
#define BUFFER 20
#define INT_MAX __INT32_MAX__
#define INT_MIN (-INT_MAX - 1)

int printStr(char *str) {
    int str_len = 0;
    while (str[str_len] != '\0')
        str_len++;

    __asm__ __volatile__(
        "movl $1, %%eax \n\t"
        "movq $1, %%rdi \n\t"
        "syscall \n\t"
        :
        : "S"(str), "d"(str_len));

    return str_len;
}

int readInt(int *str) {
    char in[BUFFER];
    int str_len;

    __asm__ __volatile__ (
        "movl $0, %%eax \n\t" 
        "movq $0, %%rdi \n\t"
        "syscall \n\t"
        : "=a"(str_len)
        :"S"(in), "d"(BUFFER));

    if(str_len < 0) {
        *str = ERR;
        return 0;
    }
    
    int sign = 1, idx = 0;
    long long int ret = 0;
    if(idx < str_len) {
        if(in[idx] == '-') {
            sign = -1;
            idx++;
        } else if(in[idx] == '+') {
            idx++;
        }
    }
    
    while(idx < str_len && in[idx] != '\n') {
        if(in[idx] < '0' || in[idx] > '9') {
            *str = ERR;
            return 0;
        }
        int current = (int)(in[idx] - '0');
        ret *= 10;
        ret += (sign * current);
        if(ret > INT_MAX || ret < INT_MIN) {
            *str = ERR;
            return 0;
        }
        idx++;
    }

    *str = OK;
    return (int)ret;
}

int printInt(int num) {
    int str_len = 0;
    long long int num1 = num;
    char result[BUFFER];
    if (num1 < 0) {
        result[str_len++] = '-';
        num1 = -num1;
    }
    while (num1) {
        result[str_len++] = (char)('0' + (num1 % 10));
        num1 /= 10;
    }
    if (str_len == 0)
        result[str_len++] = '0';

    int back = str_len - 1;
    int front = (result[0] == '-' ? 1 : 0);
    while (front < back) {
        char temp = result[front];
        result[front++] = result[back];
        result[back--] = temp;
    }

    __asm__ __volatile__(
        "movl $1, %%eax \n\t"
        "movq $1, %%rdi \n\t"
        "syscall \n\t"
        :
        : "S"(result), "d"(str_len));
    return str_len;
}
