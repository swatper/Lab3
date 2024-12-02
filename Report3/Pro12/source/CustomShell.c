#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#pragma region CommandFunc
#include "ex_rmdir.h" //rmdir 함수 헤더
#include "ex_ln.h"
#include "ex_cp.h"

#include "ChangeDirectory.h"
#include "MakeDirectory.h"
#include "WorkingDirectory.h"
#include "FileList.h"

#include "cat.h"
#include "mv.h"
#include "rm.h"
#pragma endregion

#define MAX_BUF 256
#define MAX_ARG 50

// ----------------------------------------------------
// SIGINT 핸들러 (Ctrl-C)
// ----------------------------------------------------
void handle_sigint_parent(int sig) {
    printf("\nSIGINT 수신 - 작업 중단. 쉘은 계속 실행됩니다.\n");
}

void handle_sigint_child(int sig) {
    printf("\n작업 종료\n");
    exit(0); // 자식 프로세스 종료
}
void handle_sigquit(int sig){
    printf("\n작업 종료\n");
}
// ----------------------------------------------------
// 사용자 입력을 공백으로 분리하여 인자 배열에 저장
// ----------------------------------------------------
int getargs(char *cmd, char **argv)
{
    int narg = 0;
    while (*cmd)
    {
        if (*cmd == ' ' || *cmd == '\t')
        {
            *cmd++ = '\0';
        }
        else
        {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
            {
                cmd++;
            }
        }
    }
    argv[narg] = NULL;
    return narg;
}

// ----------------------------------------------------
// 명령어 실행 함수 (파일 재지향 및 파이프 처리 포함)
// ----------------------------------------------------
void execute_command(char **argv, int input_fd, int output_fd)
{
    if (output_fd != 1 && strcmp(argv[0], "cat") != 0) {
        dup2(output_fd, STDOUT_FILENO);
        dup2(output_fd, STDERR_FILENO);
        close(output_fd);
    }

    if (strcmp(argv[0], "rmdir") == 0)
    {
        execute_rmdir(argv);
        exit(0);
    }
    if (strcmp(argv[0], "ln") == 0)
    {
        execute_ln(argv);
        exit(0);
    }
    if (strcmp(argv[0], "cp") == 0)
    {
        execute_cp(argv);
        exit(0);
    }
    if (strcmp(argv[0], "cd") == 0)
    {
        if (input_fd == 1)
        {
            ChangeDirectory(NULL);
        }
        else
        {
            ChangeDirectory(argv[1]);
        }
        exit(0);
    }
    if (strcmp(argv[0], "pwd") == 0)
    {
        WorkingDirectory(argv);
        exit(0);
    }
    if (strcmp(argv[0], "mkdir") == 0)
    {
        if (input_fd == 1)
        {
            MakeDirectory(NULL);
        }
        else
        {
            MakeDirectory(argv[1]);
        }
        exit(0);
    }
    
    if (strcmp(argv[0], "mv") == 0)
    {
        if (argv[1] == NULL || argv[2] == NULL)
        {
            printf("Usage: mv [source] [destination]\n");
        }
        else
        {
            execute_mv(argv[1], argv[2]);
        }
        exit(0);
    }
    if (strcmp(argv[0], "cat") == 0) {
        char buffer[4096];
        ssize_t bytes_read;

        // 파일 인자가 없는 경우: 표준 입력 읽기
        if (argv[1] == NULL || (argv[1] && strcmp(argv[1], ">") == 0)) {
            while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
                if (write(output_fd, buffer, bytes_read) == -1) {
                    perror("write");
                    exit(1);
                }
            }
            if (bytes_read == -1) {
                perror("read");
                exit(1);
            }
        }
    }
    if (strcmp(argv[0], "rm") == 0)
    {
        if (argv[1] == NULL)
        {
            printf("Usage: rm [filename]\n");
        }
        else
        {
            execute_rm(argv[1]);
        }
        exit(0);
    }
    if (strcmp(argv[0], "ls") == 0)
    {
        if (argv[1] == NULL)
        {
            // 인자가 없을 경우 현재 디렉토리 출력
            FileList(".");
        }
        else
        {
            // 인자가 있을 경우 각 인자를 처리
            for (int i = 1; argv[i] != NULL; i++)
            {
                struct stat statbuf;
                if (stat(argv[i], &statbuf) == -1)
                {
                    // 경로가 잘못되었거나 파일/디렉토리를 찾을 수 없음
                    perror(argv[i]);
                    continue;
                }

                if (S_ISDIR(statbuf.st_mode))
                {
                    // 디렉토리일 경우 FileList 호출
                    printf("\n목록 (%s):\n", argv[i]);
                    FileList(argv[i]);
                }
                else if (S_ISREG(statbuf.st_mode))
                {
                    // 파일일 경우 이름 출력
                    printf("파일: %s\n", argv[i]);
                }
                else
                {
                    // 기타 타입 처리
                    printf("알 수 없는 타입: %s\n", argv[i]);
                }
            }
        }
        exit(0);
    }

    if (input_fd != 0)
    {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output_fd != 1)
    {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
    execvp(argv[0], argv);
    perror("명령어 실행 실패");
    exit(1);
}

// ----------------------------------------------------
// 명령어 파이프 및 파일 재지향 처리
// ----------------------------------------------------
void process_command(char *cmd) {
    char *commands[MAX_ARG];
    int n_commands = 0;
    int pipes[MAX_ARG][2];
    int input_fd = 0, output_fd = 1;

    char *token = strtok(cmd, "|");
    while (token != NULL) {
        commands[n_commands++] = token;
        token = strtok(NULL, "|");
    }

    for (int i = 0; i < n_commands; i++) {
        char *argv[MAX_ARG];
        char *redir_file;
        input_fd = (i == 0) ? 0 : pipes[i - 1][0];
        output_fd = 1;

        if (i < n_commands - 1) {
            pipe(pipes[i]);
            output_fd = pipes[i][1];
        }

        char *cmd_with_redir = strtok(commands[i], ">");
        redir_file = strtok(NULL, ">");
        if (redir_file != NULL) {
            redir_file = strtok(redir_file, " ");
            output_fd = open(redir_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1) {
                perror("파일 열기 실패");
                return;
            }
        }

        cmd_with_redir = strtok(cmd_with_redir, "<");
        char *input_file = strtok(NULL, "<");
        if (input_file != NULL) {
            input_file = strtok(input_file, " ");
            input_fd = open(input_file, O_RDONLY);
            if (input_fd == -1) {
                perror("파일 열기 실패");
                return;
            }
        }

        getargs(cmd_with_redir, argv);

        pid_t pid = fork();
        if (pid == 0) {
            // 자식 프로세스에서 SIGINT 핸들러 설정
            signal(SIGINT, handle_sigint_child);

            if (i < n_commands - 1) {
                close(pipes[i][0]);
            }
            execute_command(argv, input_fd, output_fd);
        } else if (pid > 0) {
            // 부모 프로세스에서 SIGINT 무시
            signal(SIGINT, handle_sigint_parent);

            if (input_fd != 0) close(input_fd);
            if (output_fd != 1) close(output_fd);
            if (i < n_commands - 1) close(pipes[i][1]);

            wait(NULL); // 자식 프로세스 종료 대기
        } else {
            perror("포크 실패");
            return;
        }
    }
}
// ----------------------------------------------------
// 메인 함수
// ----------------------------------------------------
int main() {
    char buf[MAX_BUF];
    int is_background;

    signal(SIGINT, handle_sigint_parent); // 부모 핸들러 설정
    signal(SIGQUIT, handle_sigquit);

    printf("**********커스텀 쉘 실행**********\n");

    while (1) {
        printf("CustomShell> ");
        if (fgets(buf, MAX_BUF, stdin) == NULL) {
            printf("\n");
            break;
        }

        buf[strcspn(buf, "\n")] = '\0';

        if (strcmp(buf, "exit") == 0) {
            printf("**********커스텀 쉘 종료**********\n");
            break;
        }

        is_background = 0;
        if (buf[strlen(buf) - 1] == '&') {
            is_background = 1;
            buf[strlen(buf) - 1] = '\0';
        }

        process_command(buf);
    }

    return 0;
}