// gcc -S t1.c 
// gcc t1.s
// ./a.out

int var1 = 5;
int var2 = 8;
int x = 0;

#include <stdio.h>
int main(){
    x = var1 + var2;
    printf("%d", x);
}
