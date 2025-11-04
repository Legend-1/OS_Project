#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int run_pipeline(char ***cmds_args, char **infiles, char **outfiles, int n, int bg) {
    if (n <= 0) return -1;
    int *pids = malloc(sizeof(int) * n);
    int (*pipes)[2] = NULL;
    if (n > 1) {
        pipes = malloc(sizeof(int[2]) * (n-1));
        for (int i = 0; i < n-1; ++i) if (pipe(pipes[i]) < 0) { perror("pipe"); free(pids); free(pipes); return -1; }
    }
    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); }
        if (pid == 0) {
            if (i > 0) {
                dup2(pipes[i-1][0], 0);
            } else if (infiles[i]) {
                int fd = open(infiles[i], O_RDONLY);
                if (fd < 0) { perror("open in"); _exit(1); }
                dup2(fd, 0); close(fd);
            }
            if (i < n-1) {
                dup2(pipes[i][1], 1);
            } else if (outfiles[i]) {
                int fd = open(outfiles[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) { perror("open out"); _exit(1); }
                dup2(fd, 1); close(fd);
            }
            if (pipes) for (int j = 0; j < n-1; ++j) { close(pipes[j][0]); close(pipes[j][1]); }
            execvp(cmds_args[i][0], cmds_args[i]);
            perror("exec");
            _exit(1);
        } else {
            pids[i] = pid;
        }
    }
    if (pipes) for (int j = 0; j < n-1; ++j) { close(pipes[j][0]); close(pipes[j][1]); }
    if (!bg) for (int i = 0; i < n; ++i) waitpid(pids[i], NULL, 0);
    free(pids);
    if (pipes) free(pipes);
    return 0;
}
