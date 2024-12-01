#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h> //시간 관련 가져오기
#define PASSWORDSIZE 12
int main(void){
    int fd;
    int nread, cnt=0, errcnt=0;
    //타자칠 문장
    char ch;
    char *texts[] = {"The magic thing is that you can change it",
                    "I'm the best",
                    "Hello World!",
                    "Face The Fear Build The Future"};
    struct termios init_attr, new_attr;
    time_t startT, endT;
    double WPM;
    //터미널 속성 설정
    fd = open(ttyname(fileno(stdin)), O_RDWR);
    tcgetattr(fd, &init_attr);
    new_attr = init_attr;
    new_attr.c_lflag &= ~ICANON;
    new_attr.c_lflag &= ~ECHO; /* ~(ECHO | ECHOE | ECHOK | ECHONL); */  
    new_attr.c_cc[VMIN] = 1;
    new_attr.c_cc[VTIME] = 0;
    if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
        fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
    }

    //랜덤 문장 뽑기
    int numTexts = sizeof(texts) / sizeof(texts[0]);
    srand(time(NULL));
    char *text = texts[rand() % numTexts];

    printf("다음 문장을 그대로 입력하세요.\n%s\n", text);
    //측정 시작
    time(&startT);
    while ((nread=read(fd, &ch, 1)) > 0 && ch != '\n') {
        if (ch == text[cnt++])
            write(fd, &ch, 1);
        else {
            write(fd, "*", 1);
            errcnt++;
        }
    }    
    //측정 종료
    time(&endT);
    //게산
    double cosT =difftime(endT, startT) / 60.0; 
    double wordsType = (double)cnt / 5;
    WPM =  wordsType/ cosT;

    printf("\n타이핑 오류의 횟수는 %d\n", errcnt);
    printf("분당 타자수(WPM): %.0f \n", WPM);
    tcsetattr(fd, TCSANOW, &init_attr);
    close(fd);
}