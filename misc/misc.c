
#include<stdio.h>

/*

MOVI R0, b00000001
MOVI R1, b00000010
ADD R2, R0, R1
HALT

*/

int main()
{
    register long r8 asm("r8");
    register long r9 asm("r9");
    register long r12 asm("r12");


    printf("--- Misc ---\n");
    __asm__ ( 
        "mov $1, %r8;"
        "mov $2, %r9;"
        "add %r8, %r12;"
        "add %r9, %r12;"
    );

    int x = r12;

    printf("Value: %d \n", x);
    return 0;
}