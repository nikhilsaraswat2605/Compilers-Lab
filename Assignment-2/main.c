#include "myl.h"

#define INT_MAX __INT32_MAX__
#define INT_MIN (-INT_MAX - 1)

int main()
{
    printStr("\n----------------------------------------------------------------------------------------------\n");
    printStr("#### Testing of printStr function ####\n");
    printStr("----------------------------------------------------------------------------------------------\n");
    char *s[] = {"I am 3rd year undergraduate student at IIT Kharagpur.", "My Department is Computer Science and Engineering.", "", "    "};
    char *newline = "\n";
    for (int i = 0; i < 4; i++)
    {
        printStr(newline);
        printStr("\"");
        int number_of_chars = printStr(s[i]);
        printStr("\" -> number of characters printed = ");
        printInt(number_of_chars);
        printStr(newline);
    }
    printStr("\n----------------------------------------------------------------------------------------------\n");
    printStr("#### Testing of printInt function ####\n");
    printStr("----------------------------------------------------------------------------------------------\n");
    int integers[] = {INT_MAX, INT_MIN, 11, 212, -323, -324324, 0, 3123, 332432, 43243, 989797, -432, -4234};
    int n = sizeof(integers) / sizeof(int);
    for (int i = 0; i < n; i++)
    {
        printStr(newline);
        int number_of_chars = printInt(integers[i]);
        printStr(" -> number of characters printed = ");
        printInt(number_of_chars);
        printStr(newline);
    }
    printStr("\n----------------------------------------------------------------------------------------------\n");
    printStr("#### Testing of printFlt function ####\n");
    printStr("----------------------------------------------------------------------------------------------\n");
    float floating_point_numbers[] = {0.242F, -32.232F, -0.232F, 0.F, 0232.232F, -0.F, 434.43F, .343F};
    n = sizeof(floating_point_numbers) / sizeof(float);
    for (int i = 0; i < n; i++)
    {
        printStr(newline);
        int number_of_chars = printFlt(floating_point_numbers[i]);
        printStr(" -> number of characters printed = ");
        printInt(number_of_chars);
        printStr(newline);
    }
    printStr("\n----------------------------------------------------------------------------------------------\n");
    printStr("#### Testing of readInt function ####\n");
    printStr("----------------------------------------------------------------------------------------------\n");
    int rep = 0;
    do
    {
        int number;
        printStr("Enter an integer number: ");
        if (readInt(&number) == OK)
        {
            printStr("Successfuly read integer: ");
            printInt(number);
        }
        else
        {
            printStr("Invalid input. Please try again.\n");
        }
        printStr("\nType 0 to exit otherwise 1 to continue reading integer number : ");
        readInt(&rep);
    } while (rep != 0);
    printStr("\n----------------------------------------------------------------------------------------------\n");
    printStr("#### Testing of readFlt function ####\n");
    printStr("----------------------------------------------------------------------------------------------\n");
    rep = 0;
    do
    {
        float number;
        printStr("Enter a floating point number: ");
        if (readFlt(&number) == OK)
        {
            printStr("Successfuly read floating point number: ");
            printFlt(number);
        }
        else
        {
            printStr("Invalid input. Please try again.\n");
        }
        printStr("\nType 0 to exit otherwise 1 to continue reading floating point number : ");
        readInt(&rep);
    } while (rep != 0);

    return 0;
}