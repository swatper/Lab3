#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void alarm_handler(int);

int alarm_flag = 0;

int main(){ 
    struct sigaction act;
    act.sa_handler = alarm_handler;
    sigaction(SIGALRM, &act, NULL);
    alarm (5); //알람 설정
    pause(); //프로세스 정지
    if (alarm_flag)
        printf("5초 경과.\n");
    }

void alarm_handler(int sig){
    printf("알람 시그널 받음.\n");
    alarm_flag = 1;
}