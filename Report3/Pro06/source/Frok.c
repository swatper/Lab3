#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
    pid_t pid; /* 부모에서 프로세스 식별번호 저장 */
    printf("Frok 함수 실행: 자식 프로세스 생성\n");
    pid = fork(); /* 새로운 프로세스 생성 */
    if (pid == 0) printf("난 프로세스 ID가 0인 자식 프로세스\n");
    else if (pid > 0) printf("난 부모 프로세스\n");
    else printf("프로세스 생성 실패n");
} 