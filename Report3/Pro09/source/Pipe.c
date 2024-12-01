#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MSGSIZE 16

int main(){
    char buf[MSGSIZE];
    int p[2], i;
    int pid;
    int status;
    /* open pipe */
    if (pipe(p) == -1) {
        perror ("파이프 생성 실패\n");
        exit(1);
    }
    pid = fork();
    if (pid == 0) { /* child process */
        printf("부모 프로세스: 파이프 입구 폐쇄\n");
        close(p[0]);
        printf("부모 프로세스: 파이프 내용 쓰기\n");
        for (i = 0; i < 2; i++) {
            sprintf(buf, "Hello, world #%d", i+1);
            write(p[1], buf, MSGSIZE);
        }
    }
    else if (pid > 0) {
        printf("자식 프로세스: 파이프 출구 폐쇄\n");
        close(p[1]);
        wait(&status);    //부모 잠시 기다리기
        printf("자식 프로세스: 파이프 내용 읽기\n");
        for (i = 0; i < 2; i++) {
            read (p[0], buf, MSGSIZE);
            printf ("%s\n", buf);
        }
    }
    else perror ("자식 프로세스 생성 실패\n");
}