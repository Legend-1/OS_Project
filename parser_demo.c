#include <string.h>

#define MAXARGS 128

void parse_tokens(char *line, char **args) {
    int i = 0;
    char *tok = strtok(line, " \t\n");
    while (tok && i < MAXARGS-1) {
        args[i++] = tok;
        tok = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}
