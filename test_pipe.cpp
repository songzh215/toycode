#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    int pipefd[2];
    pid_t cpid;
    pid_t cpid1;
    setbuf(stdout, NULL);
    printf("pid of myself is %d", getpid());
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {
        // 关闭读端
        close(pipefd[0]);
        if (pipefd[1] != STDOUT_FILENO) {
            if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                perror("dup2 error");
                exit(EXIT_FAILURE);
            }
            if (close(pipefd[1]) == -1) {
                perror("close pipe[1] error");
                exit(EXIT_FAILURE);
            }
        }
        //fcntl(pipefd[0], F_SETPIPE_SZ, 1);
        //fcntl(STDOUT_FILENO, F_SETPIPE_SZ, 1);
        //int time = sleep(30);
        //printf("sleep left about %d time", time);
    //fcntl(pipefd[1], F_SETFD, 1); 
        execlp("ls", "ls", (char*)NULL);
        exit(EXIT_FAILURE);
    }
    printf("fork child process %d", cpid);
    cpid1 = fork();
    if (cpid1 == -1) {
        perror("fork2");
        exit(EXIT_FAILURE);
    }
    if (cpid1 == 0) {
        // 关闭写端
        close(pipefd[1]);
        //int flags = fcntl(pipefd[0], F_GETFL);//先获取原先的flags
        //fcntl(pipefd[0],F_SETFL,flags | O_NONBLOCK);//设置fd为阻塞模式
        if (pipefd[0] != STDIN_FILENO) {
            if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                perror("dup2 error");
                exit(EXIT_FAILURE);
            }
            if (close(pipefd[0]) == -1) {
                perror("close pipe[1] error");
                exit(EXIT_FAILURE);
            }
        }
        printf("WTF\n");
        execlp("wc","wc", "-l", (char*)(NULL));
        exit(EXIT_FAILURE);
    }
    printf("fork child process %d", cpid1);
    close(pipefd[0]);
    close(pipefd[1]);
    int pid1 = wait(NULL);
    printf("child process %d exit!", pid1);
    int pid2 = wait(NULL);
    printf("child process %d exit!", pid2);
}
