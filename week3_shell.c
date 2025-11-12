#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HISTFILE ".my_history"
#define MAXHIST 500

char *history_list[MAXHIST];
int history_count = 0;

void history_load() {
    FILE *f = fopen(HISTFILE, "r");
    if (!f) return;
    char buf[1024];
    while (fgets(buf, sizeof(buf), f)) {
        buf[strcspn(buf, "\n")] = 0;
        if (history_count < MAXHIST) history_list[history_count++] = strdup(buf);
    }
    fclose(f);
}

void history_add_and_persist(const char *line) {
    if (!line || line[0]==0) return;
    if (history_count < MAXHIST) history_list[history_count++] = strdup(line);
    FILE *f = fopen(HISTFILE, "a");
    if (!f) return;
    fprintf(f, "%s\n", line);
    fclose(f);
}

void history_show() {
    for (int i = 0; i < history_count; ++i) {
        printf("%d %s\n", i+1, history_list[i]);
    }
}
