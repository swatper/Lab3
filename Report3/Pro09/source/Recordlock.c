#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    int fd;
    struct flock testlock;
    int pid;
    /* 쓰기 잠금의 인자 지정 */
    testlock.l_type = F_WRLCK;
    testlock.l_whence = SEEK_SET;
    testlock.l_start = 0;
    testlock.l_len = 10;
    /* open file */
    fd = open ("testlock.txt", O_RDWR | O_CREAT, 0666);
    if (fcntl (fd, F_SETLKW, &testlock) == -1) {
        perror ("parent: locking");
        exit (1);
    }
    printf ("부모 프로세스: 레코드 락\n");
    pid = fork();
    if (pid == 0) { /* child process */
        testlock.l_len = 5;
        if (fcntl (fd, F_SETLKW, &testlock) == -1) {
            perror ("child: locking");
            exit (1);
        }
        printf ("자식 프로세스: 레코드 락\n");
        sleep(5);
        printf ("자식 프로세스 종료\n");
    }
    else if (pid > 0) {
        sleep(5);
        printf ("부모 프로세스 종료\n");
    }
    else
        perror ("frok 실패\n");
}