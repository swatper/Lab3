#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 128
#define QKEY (key_t)0111 //메시지 큐 고유 번호

struct msgq_data {
    long type;
    char text[BUFSIZE];
};

//메시지 보내기
void send_message(int qid, long type) {
    struct msgq_data send_data;
    send_data.type = type; 
    //type==1: 부모 프로새스, type==2: 자식 프로세스
    printf("%s", (type == 1 ? "부모 프로세스: " : "자식 프로세스: "));
    fgets(send_data.text, BUFSIZE, stdin);
    send_data.text[strcspn(send_data.text, "\n")] = '\0'; // 개행 문자 제거

    if (msgsnd(qid, &send_data, strlen(send_data.text) + 1, 0) == -1) {
        perror("메시지 전달 실패");
        exit(1);
    }
}

//메시지 받기
void receive_message(int qid, long type) {
    struct msgq_data recv_data;
    if (msgrcv(qid, &recv_data, BUFSIZE, type, 0) == -1) {
        perror("메시지 받기 실패");
        exit(1);
    }
    //type==1: 부모 프로새스, type==2: 자식 프로세스
    //받은 메시지 확인용 코드
    printf("%s: %s\n", (type == 1 ? 
           "부모 프로세스로부터 받음" : "자식 프로세스로부터 받음"), recv_data.text);
}

int main() {
    int qid;
    pid_t pid;

    // 메시지 큐 생성
    if ((qid = msgget(QKEY, IPC_CREAT | 0666)) == -1) {
        perror("메시지 큐 생성 실패");
        exit(1);
    }

    // 자식 프로세스 생성
    if ((pid = fork()) < 0) {
        perror("자식 프로세스 생성 실패");
        exit(1);
    }

    //부모 프로세스: 메시지 먼저 보냄
    if (pid > 0) { 
        while(1){
            send_message(qid, 1);
            // 부모는 자식 메시지를 수신
            receive_message(qid, 2);
        }
    } 
    //자식 프로세스: 메시지 먼저 받음
    else if (pid == 0){
        while (1){
            receive_message(qid, 1);
            // 자식이 메시지를 보냄
            send_message(qid, 2);
        }
    }
    return 0;
}