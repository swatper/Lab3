/*   자식 프로세스 생성해서 명령어 실행하기      */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXCOMMAND 50
void execute_command(char **argv);//명령어를 실행할 함수

int main(int argc, char *argv[]){
    pid_t pid;
    int status, exit_status;
    if (argc < 1) {
        printf("명령어 인자가 부족합니다.");
        exit(1);
    }

    // 자식 프로세스 생성
    if ((pid = fork()) < 0) {
        perror("자식 프로세스 생성 실패");
        exit(1);
    }

    if(pid == 0){
        // 명령어 인자가 하나만 있을 경우
        if (argc == 2) { 
            execl("/bin/sh", "sh", "-c", argv[1], NULL);
        }
        //명령어 인지가 2개일 경우 
        else if (argc > 2) { 
            char cmd[MAXCOMMAND];
            // 인자를 합침
            snprintf(cmd, sizeof(cmd), "%s %s", argv[1], argv[2]); 
            execl("/bin/sh", "sh", "-c", cmd, NULL);
            fprintf(stderr, "명령어 실행 실패\n");
            exit(1);
        } 
    }
    else{
        //부모 프로세스는 대기
        if ((pid = wait(&status)) == -1) {
            perror("부모 프로세스 대기 실패");
            exit(2);
        }
    }
    //자식 프로세스 종료 시 부모 프로세스 종료
    if (WIFEXITED(status)) { 
        exit(0);
    }
}