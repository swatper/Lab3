#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#define SHMKEY (key_t)0111
#define SEMKEY (key_t)0222
#define SHMSIZE 1024

void p(int semid, int sem_num);
void v(int semid, int sem_num);

int main(int argc, char *argv[]) {
    int shmid, semid;
    char *shmaddr;
    FILE *src, *dst;
    pid_t pid;
    int status;
    union semun {
        int value;
        struct semid_ds *buf;
        unsigned short int *array;
    } arg;
    if (argc != 3) {
        fprintf(stderr, "인자 개수가 옯바르지 않흡니다.\n");
        exit(1);
    }
    if ((shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0666)) == -1) {
        perror("공유 메모리 생성 실패");
        exit(1);
    }
    // 세마포어 생성
    if ((semid = semget(SEMKEY, 2, IPC_CREAT | 0666)) == -1) {
        perror("세마포어 생성 실패");
        exit(1);
    }
    // 세마포어 초기화
    arg.value = 1;  // 부모 세마포어 초기화
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl failed for parent semaphore");
        exit(1);
    }
    arg.value = 0;  // 자식 세마포어 초기화
    if (semctl(semid, 1, SETVAL, arg) == -1) {
        perror("semctl failed for child semaphore");
        exit(1);
    }
    if ((pid = fork()) < 0) {
        perror("자식 프로세스 생성 실패");
        exit(1);
    }
    // 부모 프로세스: 파일 읽기고 공유 메모리에 적기
    if (pid > 0) { 
        if ((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1) {
            perror("shmat failed");
            exit(1);
        }
        printf("부모 프로세스: 파일 내용 읽기\n");
        if ((src = fopen(argv[1], "r")) == NULL) {
            perror("fopen failed for source file");
            exit(1);
        }
        char buffer[SHMSIZE];
        while (fgets(buffer, SHMSIZE, src) != NULL) {
            printf("부모 프로세스: 세마포어 잠금 \n");
            p(semid, 0); // 부모 세마포어 P 연산 (잠금)
            printf("부모 프로세스: 읽은 내용 공유 메모리에 쓰기\n");
            strncpy(shmaddr, buffer, SHMSIZE); // 공유 메모리에 데이터 복사
            printf("부모 프로세스: 세마포어 잠금 해제\n");
            v(semid, 1);  // 자식 세마포어 V 연산 (해제)
        }
        // 자식 프로세스를 기다리기 위한 코드
        fclose(src);
        p(semid, 0);  // 부모 세마포어 잠금
        shmaddr[0] = '\0'; 
        v(semid, 1);  // 자식 세마포어 해제
        wait(&status);
        // 공유 메모리 해제
        if (shmdt(shmaddr) == -1) {
            perror("shmdt failed");
            exit(1);
        }
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl failed");
            exit(1);
        }
        // 세마포어 해제
        if (semctl(semid, 0, IPC_RMID, arg) == -1) {
            perror("semctl failed");
            exit(1);
        }
    } 
    // 자식 프로세스: 공유 메모리 내용 읽고 파일에 쓰기
    else if (pid == 0){
        printf("자식 프로세스: 파일에 내용 적을 준비\n"); 
        if ((dst = fopen(argv[2], "w")) == NULL) {
            perror("fopen failed for destination file");
            exit(1);
        }
        printf("자식 프로세스: 공유 메모리 접근\n");
        if ((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1) {
            perror("shmat failed");
            exit(1);
        }
        while (1) {
            printf("자식 프로세스: 세마포어 잠금\n");
            p(semid, 1);  ;  // 자식 세마포어 P 연산 (잠금)
            printf("자식 프로세스: 파일에 내용 쓰기\n");     
            if (shmaddr[0] == '\0') break; 
            fprintf(dst, "%s", shmaddr);
            printf("자식 프로세스: 세마포어 잠금 해제\n");
            v(semid,0);  // 부모 세마포어 V 연산 (해제)
        }
        fclose(dst);
        // 공유 메모리 해제
        if (shmdt(shmaddr) == -1) {
            perror("shmdt failed");
            exit(1);
        }
        exit(0);
    }
    return 0;
}

// P 연산 (잠금)
void p(int semid, int sem_num) {
    struct sembuf pbuf;
    pbuf.sem_num = sem_num;  // 부모 세마포어(0) 또는 자식 세마포어(1)
    pbuf.sem_op = -1;
    pbuf.sem_flg = SEM_UNDO;
    if (semop(semid, &pbuf, 1) == -1) {
        perror("semop failed");
        exit(1);
    }
}

// V 연산 (잠금 해제)
void v(int semid, int sem_num) {
    struct sembuf vbuf;
    vbuf.sem_num = sem_num;  // 부모 세마포어(0) 또는 자식 세마포어(1)
    vbuf.sem_op = 1;
    vbuf.sem_flg = SEM_UNDO;
    if (semop(semid, &vbuf, 1) == -1) {
        perror("semop failed");
        exit(1);
    }
}