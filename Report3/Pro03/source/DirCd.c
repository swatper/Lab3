#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_BUF 256
int main(int argc, char *argv[]){
    char buf[MAX_BUF];
    if (argc < 2) {
        fprintf(stderr, "Usage: file_chdir dirname\n");
        exit(1);
    }
    memset(buf, 0, MAX_BUF);
    if (getcwd(buf, MAX_BUF) < 0) { //pwd명령어
        perror("getcwd");
        exit(1);
    }
    printf("working directory (before) = %s\n", buf);
    if (chdir(argv[1]) < 0) {      //cd명령어
        perror("chdir");
        exit(1);
    }
    memset(buf, 0, MAX_BUF);      
    if (getcwd(buf, MAX_BUF) < 0) {//pwd명령어
        perror("getcwd");
        exit(1);
    }
    printf("working directory (after ) = %s\n", buf);
}  