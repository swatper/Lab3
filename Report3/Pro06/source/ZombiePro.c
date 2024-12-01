#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("folk");
        exit(1);
    }
    else if (pid == 0) {
        printf("자식 프로세스 종료\n");
        exit(0);
    }
    printf("부모 프로세스 4초간 대기\n");
    sleep(4);
    printf("현재 실행중인 프로세스 확인\n");
    system("ps -o pid,ppid,state,tty,command");
    exit(0);
}