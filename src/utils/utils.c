#include <stdio.h>

int get_int_input(char *message) {
    int input;
    printf("\033[0;33m%s\n\033[0m", message);
    scanf("%d", &input);
    return input;
}
