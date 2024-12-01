#include<stdio.h>
#include <stdlib.h>

int main(){
    int exit_status;
    printf("종료 상태를 입하세요: ");
    scanf("%d", &exit_status);
    exit(exit_status);
}