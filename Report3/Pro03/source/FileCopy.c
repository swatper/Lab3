#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#define MAX_READ 10

int main(int argc, char * argv[]){
    int src_fd;
    int dst_fd;
    char buf[MAX_READ];
    ssize_t rcnt;
    ssize_t tot_cnt = 0;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if (argc < 3){
        fprintf(stderr, "Usage: Copyexecute src_file dest_file \n");
    }

    if((src_fd = open(argv[1], O_RDONLY)) == -1){
        perror("src open");
        return 0;
    }

    if((dst_fd = creat(argv[2],mode)) == -1){
        perror("dst open");
        return 0;
    }

    while((rcnt = read(src_fd,buf,MAX_READ))>0){
        tot_cnt += write(dst_fd,buf,rcnt);
    }

    if (rcnt < 0){
        perror("read");
        return 0;
    }
    
    printf("total write cout = %d \n",tot_cnt);
    close(src_fd);
    close(dst_fd);
    
    return 0;
}