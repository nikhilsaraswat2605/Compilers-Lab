#include "myl.h"

#ifndef BUFF_SIZE
#define BUFF_SIZE 32
#endif

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
