#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(){
    sigset_t set1, set2;
    /* 모든 시그널을 갖고 있는 set1 설정*/
    sigfillset(&set1);
    /* 빈 시그널 집합 set2 설정*/
    sigemptyset(&set2);
    /*set2에 SIGIMT 시그널 추가*/
    sigaddset(&set2, SIGINT);
    printf("5초간 시그널 차단\n");
    sigprocmask(SIG_BLOCK, &set1, NULL); 
    sleep(5);
    printf("set2의 시그널 차단 해제\n");
    sigprocmask(SIG_UNBLOCK, &set2, NULL); 
    sleep(5);
    printf("모든 시그널 차단 해제.\n");
    sigprocmask(SIG_UNBLOCK, &set1, NULL);
    sleep(5);
}