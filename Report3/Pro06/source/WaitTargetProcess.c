#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    pid_t pid;
    int status, exit_status;
    if ((pid = fork()) < 0) printf("프로세스 생성 실패\n");
    if (pid == 0){
        printf("자식 프로세스 4초간 대기\n");
        sleep (4); 
        printf("자식 프로세스 종료\n");
        exit (5);
    }
    while (waitpid (pid, &status, WNOHANG) == 0) {
        printf ("특정 자식 종료 확인 후 1초간 대기\n");
        sleep (1);
    }
    if (WIFEXITED(status)){
        exit_status = WEXITSTATUS (status);
        printf ("부모 프로세스 상태 변경: %d에서  %d\n",pid, exit_status);
    }
    exit (0);
}