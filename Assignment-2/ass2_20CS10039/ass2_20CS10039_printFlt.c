#include "myl.h"

#ifndef BUFF_SIZE
#define BUFF_SIZE 32
#endif

int printFlt(float f)
{
    long long int integer_part = (long long int)f;                                           // get the integer part of the number
    long long int fractional_part = (long long int)((f - (float)integer_part) * 1000000000); // get the fractional part of the number with 6 decimal places
    if (f < 0)
    {                                       // if the number is negative
        fractional_part = -fractional_part; // make the fractional part positive
    }
    while (fractional_part > 0)
    { // if the fractional part is negative, make it positive
        if (fractional_part % 10 == 0 || fractional_part >= 10000000)
        {                          // if the fractional part is divisible by 10
            fractional_part /= 10; // divide by 10
        }
        else if (fractional_part < 10000000)
        { // if the fractional part is not divisible by 10
            break;
        }
    }
    int numchar = printInt(integer_part); // print the integer part of the number
    numchar += printStr(".");             // print the decimal point
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
