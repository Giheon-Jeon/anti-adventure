#include <stdio.h>
#include <stdlib.h>
#include "../include/utils.h"

int get_random_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void wait_for_enter() {
    printf("\n엔터를 누르면 계속합니다...");
    getchar();
}
