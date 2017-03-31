#include <unistd.h>
#include <stdio.h>


int main() {
    int test_var = 0;
    pid_t pid = fork();
    if(pid < 0) {
        perror("fork error");
    } else if (pid == 0) {
        double abc = 998111;
        double hhh = 998111;
        test_var = 123;
        printf("son process address test_var %p , test_var is %d \n", static_cast<void*>(&test_var), test_var);
        printf("son process address abc %p , abc is %f \n", static_cast<void*>(&abc), abc);
        printf("son process address abc %p , abc is %f \n", static_cast<void*>(&hhh), hhh);
    } else {
        bool def = true;
        bool zzz = true;
        printf("parent process address test_var %p test var is %d \n", static_cast<void*>(&test_var), test_var);
        printf("parent process address def %p def is %d \n", static_cast<void*>(&def), def);
        printf("parent process address def %p def is %d \n", static_cast<void*>(&zzz), zzz);
    }
}
