#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <signal.h>
#include <unistd.h>

void handler(int signo);
//시그널 호출 횟수를 저정할 변수
int sig_Count = 0;

int main(){
    struct sigaction sig;
    int i = 0;
    sig.sa_handler = handler; //SIG_IGN 사용 시 signal 무시
    sigfillset(&(sig.sa_mask));
    sigaction(SIGINT, &sig, NULL); 
    printf("SIGINT on\n");
    while(1) {
        sleep(1);
        printf("총 Sleep 시간: %d초.\n", ++i);
    }
}

void handler(int signo){
    sig_Count++;
    //프로세스 종료
    if(sig_Count >=2){
        exit(1);
    }
    printf("시그널 번호=%d\n", signo);
}