#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror(NULL);
     
        return errno;
    }
    if (pid == 0) {
        
        if (execve("/bin/ls", argv, NULL) == -1) {
            perror(NULL);
            return errno;
        }

        printf("Child %d finished\n", getpid());
    }
    else {
    printf("My PID = %d, Child PID = %d\n", pid, getpid());
    }
    return 0;
}