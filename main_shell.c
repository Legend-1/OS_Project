#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXLINE 2048
#define MAXSEGS 128

void parse_tokens(char *line, char **args);
void demo_exec_ls();
int spawn_and_exec(char **args, int infd, int outfd, int bg);
void demo_fork_print();
void execute_simple(char **args, int infd, int outfd, int bg);
void history_load();
void history_add_and_persist(const char *line);
void history_show();
void parse_segment(char *seg, char **args, char **infile, char **outfile);
int run_pipeline(char ***cmds_args, char **infiles, char **outfiles, int n, int bg);
char *trim_whitespace(char *s);

int main() {
    history_load();
    char line[MAXLINE];
    while (1) {
        printf("shell@osproject$ ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0;
        char *s = trim_whitespace(line);
        if (!s || s[0] == 0) continue;
        history_add_and_persist(s);
        int background = 0;
        int len = strlen(s);
        if (len > 0 && s[len-1] == '&') {
            background = 1;
            s[len-1] = 0;
            s = trim_whitespace(s);
        } else {
            char *amp = strrchr(s, '&');
            if (amp) {
                char *t = amp + 1;
                int onlyws = 1;
                while (*t) { if (*t != ' ' && *t != '\t' && *t != '\n') { onlyws = 0; break; } t++; }
                if (onlyws) { background = 1; *amp = 0; s = trim_whitespace(s); }
            }
        }
        if (strcmp(s, "exit") == 0) break;
        if (strcmp(s, "history") == 0) { history_show(); continue; }
        if (strncmp(s, "cd ", 3) == 0 || strcmp(s, "cd") == 0) {
            char buf[MAXLINE];
            strncpy(buf, s, MAXLINE); buf[MAXLINE-1]=0;
            char *args[128]; parse_tokens(buf, args);
            if (args[1]) { if (chdir(args[1]) != 0) perror("chdir"); }
            else { chdir(getenv("HOME")); }
            continue;
        }
        char *segments[MAXSEGS];
        int segcount = 0;
        char *tmp = strdup(s);
        char *p = strtok(tmp, "|");
        while (p && segcount < MAXSEGS) { segments[segcount++] = trim_whitespace(p); p = strtok(NULL, "|"); }
        char **cmds_args[segcount];
        char *infiles[segcount];
        char *outfiles[segcount];
        for (int i = 0; i < segcount; ++i) {
            char segcopy[MAXLINE];
            strncpy(segcopy, segments[i], MAXLINE); segcopy[MAXLINE-1]=0;
            char *args[MAXLINE];
            for (int j = 0; j < MAXLINE; ++j) args[j] = NULL;
            parse_segment(segcopy, args, &infiles[i], &outfiles[i]);
            int argc = 0; while (args[argc]) argc++;
            cmds_args[i] = malloc(sizeof(char*) * (argc + 1));
            for (int j = 0; j <= argc; ++j) cmds_args[i][j] = args[j];
        }
        if (segcount == 1) {
            char **a = cmds_args[0];
            if (!a[0]) { for (int i = 0; i < segcount; ++i) free(cmds_args[i]); free(tmp); continue; }
            execute_simple(a, infiles[0] ? open(infiles[0], O_RDONLY) : -1, outfiles[0] ? open(outfiles[0], O_WRONLY | O_CREAT | O_TRUNC, 0644) : -1, background);
        } else {
            run_pipeline(cmds_args, infiles, outfiles, segcount, background);
        }
        for (int i = 0; i < segcount; ++i) free(cmds_args[i]);
        free(tmp);
    }
    return 0;
}
