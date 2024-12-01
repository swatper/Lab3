#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

//프로토타입 선언
int getargs(char *cmd, char **argv);

int main(){
    char buf[256];
    char *argv[50];
    int narg;
    pid_t pid;
    //입력 받기
    while (1) {
        printf("shell> ");
        scanf("%s", buf);
        clearerr(stdin);
        narg = getargs(buf, argv);
        pid = fork();
        if (pid == 0)
            //명령어 실행
            execvp(argv[0], argv);
        else if (pid > 0)
            wait((int *) 0);
        else
            perror("fork failed");
    }
}

//입력 받은 명령어 파싱
int getargs(char *cmd, char **argv){
    int narg = 0;
    while (*cmd) {
        if (*cmd == ' ' || *cmd == '\t')
            *cmd++ = '\0';
        else {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
            cmd++;
        }
    }
    argv[narg] = NULL;
    return narg;
}