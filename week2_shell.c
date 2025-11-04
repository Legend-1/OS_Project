#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void execute_simple(char **args, int infd, int outfd, int bg) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return; }
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
    }
}
