#include <string.h>
#include <ctype.h>

char *trim_whitespace(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    char *e = s + strlen(s) - 1;
    while (e >= s && isspace((unsigned char)*e)) { *e = 0; e--; }
    return s;
}
