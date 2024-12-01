#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define MSGSIZE 64

char *testfifo = "fifo";

int main (int argc, char **argv){
    int fd;
    char buf[MSGSIZE];
    if (mkfifo(testfifo, 0666) == -1) {
        perror ("mkfifo failed");
    exit (1);
    }
    if ((fd = open(testfifo, O_RDWR)) < 0) {
        perror("fifo open failed");
        exit (1);
    }
    while (1) {
        if (read (fd, buf, MSGSIZE) < 0) {
            perror ("fifo read failed");
            exit (1);
        }
        printf ("received message: %s\n", buf);
    }
}