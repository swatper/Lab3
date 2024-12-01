#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#define MAX_PATH 1024
#define MAX_QUEUE 100

//큐 구조체
typedef struct {
    char *items[MAX_PATH];
    int front;
    int rear;
} Queue;

//큐 초기화
void InitQueue(Queue *q);
//큐가 비어 있는지 확인
int isEmpty(Queue *q);
//큐 원소 삽입
void Enqueue(Queue *q, const char *path);
//큐 원소 삭제
char *Dequeue(Queue *q);
//탐색 함수
void DirectoryList(const char *path);

int main(int argc, char *argv[]){   
    if (argc < 2) {
        exit(1);
    } 
    DirectoryList(argv[1]);
} 

void DirectoryList(const char *path){
    //큐 생성
    Queue dirq;
    InitQueue(&dirq);
    Enqueue(&dirq, path);
    //탐색 시작
    while (!isEmpty(&dirq)){
        DIR *pdir;
        struct dirent *pde;
        int i = 0;
        char *curPath = Dequeue(&dirq);

        //디렉토리 열기
        if ( (pdir = opendir(curPath)) == NULL) {
            perror("opendir");
            exit(1);
        }

        printf("Current directory: %s\n",curPath);
        //목록 읽기
        while ((pde = readdir(pdir)) != NULL) {
            if(strcmp(pde->d_name,".") == 0 || strcmp(pde->d_name,"..") == 0){
                continue;
            }
            printf("%20s ", pde->d_name);
            if (++i % 5 == 0) printf("\n");
        
            //읽은 파일이 만약 디렉토리면
            //디렉토리 구조채(dirent)의 d_type을 가져와서 확인
            if(pde->d_type == DT_DIR){
                char  subPath[MAX_PATH];
                //현재 경로 + 파일 이름 -> 다음 경로
                snprintf(subPath, sizeof(subPath), "%s/%s", curPath, pde->d_name);
                //큐에 저장
                Enqueue(&dirq,subPath);
            }
        }
        printf("\n");
        //디렉토리 닫음
        closedir(pdir);
    }
}

//큐 초기화
void InitQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

//큐가 비어 있는지 확인
int isEmpty(Queue *q) {
    return q->front == -1;
}
// 큐에 삽입
void Enqueue(Queue *q, const char *path) {
    if (q->rear == MAX_QUEUE - 1) {
        printf("Queue is full\n");
        return;
    }
    q->rear++;
    q->items[q->rear] = strdup(path);
    if (q->front == -1) {
        q->front = 0;
    }
}

char *Dequeue(Queue *q){
    if (isEmpty(q)) {
        return NULL;
    }

    char *item = q->items[q->front];
    if (q->front >= q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }

    return item;
}