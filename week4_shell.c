#include <string.h>

#define MAXARGS 128

void parse_segment(char *seg, char **args, char **infile, char **outfile) {
    *infile = *outfile = NULL;
    int i = 0;
    char *tok = strtok(seg, " \t\n");
    while (tok && i < MAXARGS-1) {
        if (strcmp(tok, "<") == 0) {
            tok = strtok(NULL, " \t\n");
            if (tok) *infile = tok;
        } else if (strcmp(tok, ">") == 0) {
            tok = strtok(NULL, " \t\n");
            if (tok) *outfile = tok;
        } else {
            args[i++] = tok;
        }
        tok = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}
