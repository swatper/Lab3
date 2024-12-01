#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    pid_t pid;
    int status, exit_status;
    if ((pid = fork()) < 0) perror ("포크 실패");
    if (pid == 0) {
        printf("자식 프로세스 4초간 대기\n");
        sleep(4);
        printf("자식 프로세스 종료\n");
        exit(5);
    }
    
    printf("부모 프로스: 자식 프로세스가 종료될 때까지 대기\n");
    if ((pid = wait(&status)) == -1) {
        perror("wait failed");
        exit(2);
    }
    if (WIFEXITED(status)) {
        exit_status = WEXITSTATUS(status);
        printf("부모 프로세스 상태 변경: %d에서  %d\n",pid, exit_status);
    }
    exit(0);
}