#include "myl.h"

int main(){
    char str[] = "Hello World\n\n";
    printStr(str);
    printInt(432);
    printStr("\n\n");
    int len = printFlt(3.687);
    printStr("\n\n");
    printInt(len);
}