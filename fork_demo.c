#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void demo_fork_print() {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return; }
    if (pid == 0) {
        printf("child: pid=%d ppid=%d\n", getpid(), getppid());
        _exit(0);
    } else {
        printf("parent: pid=%d child=%d\n", getpid(), pid);
    }
}
