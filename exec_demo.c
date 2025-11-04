#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void demo_exec_ls() {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return; }
    if (pid == 0) {
        char *args[] = {"ls", "-l", NULL};
        execvp(args[0], args);
        perror("execvp");
        _exit(1);
    } else {
        int st;
        waitpid(pid, &st, 0);
    }
}

int spawn_and_exec(char **args, int infd, int outfd, int bg) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return -1; }
    if (pid == 0) {
        if (infd != -1) { dup2(infd, 0); close(infd); }
        if (outfd != -1) { dup2(outfd, 1); close(outfd); }
        execvp(args[0], args);
        perror("exec");
        _exit(1);
    } else {
        if (!bg) {
            int st;
            waitpid(pid, &st, 0);
        }
        return pid;
    }
}
