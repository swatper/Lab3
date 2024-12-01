#include <stdlib.h>
#include <stdio.h>
void exitfunc1(void);
void exitfunc2(void);

int main(){
    atexit(exitfunc1);
    atexit(exitfunc2);
    printf("이것은 main에 있는 함수\n");
}

void exitfunc1(void){
    printf("이것은 exitfunc1 함수\n");
}

void exitfunc2(void){
    printf("이것은 exitfunc2 함수\n");
}