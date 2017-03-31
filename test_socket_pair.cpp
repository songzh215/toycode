#include <iostream>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char ** argv) {
    pid_t pid;
    int fd[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0,fd) == -1) {
        perror("socketpair error");
    }
    if (pid = fork() == -1) {
        perror("fork error");
    } else if (pid == 0) {
        //child
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO)
        dup2(fd[1], STDIN_FILENO)
        close(fd[1]);
    } else {
        close(fd[1]);
        dup2(fd[0], STDOUT_FILENO)
        dup2(fd[0], STDIN_FILENO)
        close(fd[0]);
    }
    return 0;
}
