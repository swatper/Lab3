#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
    printf("execl 함수로 ls 실행\n");
    //경로, 실행 파일 이름, 인자1,....
    execl("/bin/ls", "ls", "-l", NULL);
    //execl 함수가 정상 실행되면 
    //아래 코드는 실행 안 됨
    perror("execl 함수로 ls 명령어 실행 실패");
    exit(1);
}