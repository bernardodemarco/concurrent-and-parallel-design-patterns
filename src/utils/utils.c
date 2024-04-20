#include <stdio.h>

int get_int_input(char *message) {
    int input;
    printf("%s", message);
    scanf("%d", &input);
    return input;
}