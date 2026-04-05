#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    printf("\n" CYAN ">> 엔터를 누르면 계속합니다..." RESET);
    getchar();
}

void draw_hp_bar(const char* label, int current, int max, int width, const char* color) {
    float percent = (float)current / max;
    if (percent < 0) percent = 0;
    int filled_len = (int)(percent * width);
    
    // 한글 가독성 및 정렬을 위한 실제 출력 너비 계산
    int visual_width = 0;
    for (int i = 0; label[i] != '\0'; ) {
        unsigned char c = (unsigned char)label[i];
        if (c < 0x80) { // ASCII
            visual_width += 1;
            i += 1;
        } else if ((c & 0xE0) == 0xC0) { // 2 bytes
            visual_width += 2;
            i += 2;
        } else if ((c & 0xF0) == 0xE0) { // 3 bytes (한글 등)
            visual_width += 2;
            i += 3;
        } else if ((c & 0xF8) == 0xF0) { // 4 bytes
            visual_width += 2;
            i += 4;
        } else {
            i += 1; // 정의되지 않은 경우
        }
    }

    // 최소 12칸 맞춤을 위한 공백 계산
    int padding = 12 - visual_width;
    if (padding < 0) padding = 0;

    printf("%s%s" RESET, color, label);
    for (int i = 0; i < padding; i++) printf(" ");
    printf(" [");

    for (int i = 0; i < width; i++) {
        if (i < filled_len) printf("█");
        else printf("░");
    }
    printf("] %d/%d (%d%%)\n", current, max, (int)(percent * 100));
}

void print_centered(const char* text, int width) {
    int len = strlen(text);
    int padding = (width - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", text);
}

void print_divider(int width, const char* color) {
    printf("%s", color);
    for (int i = 0; i < width; i++) printf("━");
    printf(RESET "\n");
}
