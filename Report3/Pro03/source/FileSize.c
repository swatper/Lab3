#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
int main(int argc, char *argv[]){
    int fd; 
    off_t size;
    if (argc < 2) {
        fprintf(stderr, "Usage: file_size filename\n");
        return 0;
    }

    if ( (fd = open(argv[1], O_RDONLY)) == -1 ){
        perror("open"); 
        return 0;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        perror("lseek");
        return 0;
    }

    printf("%s\'s size = %d \n", argv[1], size);
    close(fd);     
    return 0;
}