#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define STRLEN 10

int main(){
    int fd, pid;
    char *mem;
    char buf[] = "Hello";
    /* open file */
    fd = open ("testlock.txt", O_RDWR | O_CREAT, 0666);
    ftruncate(fd, STRLEN);
    printf ("부모 프로세스: 파일 열기 성공, 공유 메모리 공간으로 사용\n");
    pid = fork();
    if (pid == 0) { 
        printf("자식 프로세스: 매핑 생성\n");
        mem = mmap(NULL, STRLEN, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        printf("자식 프로세스: 메모리에 %s 내용 저장\n", buf);
        memcpy(mem, buf, STRLEN);
        printf ("자식 프로세스: 매핑 해제\n");
        munmap(mem, STRLEN);
    }
    else if (pid > 0) {/* parent process */
        sleep(2);
        mem = mmap(NULL, STRLEN, PROT_READ, MAP_SHARED, fd, 0);
        printf ("부모 프로세스: 버퍼 내용=%s\n", mem);
        munmap(mem, STRLEN);
    }
    else perror ("포크 실패");
    close(fd);
}