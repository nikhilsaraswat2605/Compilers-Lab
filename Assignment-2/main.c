#include "myl.h"

int main(){
    char str[] = "Hello World\n\n";
    printStr(str);
    printStr("\nEnter an integer: ");
    int num;
    readInt(&num);
    printInt(num);
    printStr("\nEnter a float: ");
    float flt;
    readFlt(&flt);
    printFlt(flt);
    return 0;
}