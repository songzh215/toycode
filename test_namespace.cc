#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <sys/wait.h>
#include <boost/filesystem.hpp>
#include <sys/mount.h>


void print_namespace_id(pid_t my_pid) {
    char pid_str[10];
    sprintf(pid_str, "%d", my_pid);
    boost::filesystem::path ns_path =  boost::filesystem::path("/proc/") / pid_str / "ns/pid";
    boost::filesystem::path real_path = boost::filesystem::read_symlink(ns_path);
    boost::filesystem::path mnt_ns_path =  boost::filesystem::path("/proc/") / pid_str / "ns/mnt";
    boost::filesystem::path mnt_real_path = boost::filesystem::read_symlink(mnt_ns_path);
    boost::filesystem::path exe =  boost::filesystem::path("/proc/") / pid_str / "exe";
    boost::filesystem::path real_exe = boost::filesystem::read_symlink(exe);
    printf("%s pid namespace %s \n", ns_path.c_str(), real_path.c_str());
    printf("%s mnt namespace %s \n", mnt_ns_path.c_str(), mnt_real_path.c_str());
    printf("%s program %s \n", exe.c_str(), real_exe.c_str());
}

int son(void *) {
    // mount proc
    int ret = mount("/proc", "/proc", "proc", MS_MGC_VAL, NULL);
    if (ret < 0) {
        perror("error");
    }
    pid_t my_pid = getpid();
    pid_t my_ppid = getppid();
    printf("son process %d \n", my_pid);
    printf("my father process %d \n", my_ppid);
    print_namespace_id(my_pid);
    exit(0);
}

int main() {
    int flags = CLONE_NEWPID | CLONE_NEWNS;
    //int flags = CLONE_NEWNS;
    char stack[124];
    //pid_t pid = clone(son, stack + sizeof(stack), 0, NULL);
    pid_t pid = clone(son, stack + sizeof(stack), SIGCHLD | flags, NULL);
    //pid_t pid = fork();
    if (pid < 0) {
        perror("clone error");
    }
    if (pid > 0) {
        pid_t my_pid = getpid();
        printf("father process %d \n", my_pid);
        print_namespace_id(my_pid);
        //int status;
        pid_t dead_pid = wait(NULL);
        if (dead_pid < 0) {
            perror("wait error");
        }
    }/* else {
        sleep(10);
        printf("son process sleep \n");
    }*/
    printf("father ends");
    exit(0);
}
