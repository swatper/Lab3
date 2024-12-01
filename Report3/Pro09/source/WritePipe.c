#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#define MSGSIZE 64

char *testfifo = "fifo";

int main (int argc, char **argv){
    int fd, i, nwrite;
    char buf[MSGSIZE];
    if (argc < 2) {
        fprintf (stderr, "Usage: ex11_8_s msg ...\n");
    exit (1);
    }
    if ((fd = open(testfifo, O_WRONLY)) < 0) {
        perror("fifo open failed");
        exit (1);
    }
    for (i = 1; i < argc; i++) {
        strcpy (buf, argv[i]);
        if ((nwrite = write (fd, buf, MSGSIZE)) < 0) {
            perror ("fifo write failed");
            exit (1);
        }
    }
}