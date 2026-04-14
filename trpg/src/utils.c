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
    // 입력 버퍼에 남아있는 문자가 있을 수 있으므로, \n을 만날 때까지 비우거나 대기
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_visual_width(const char* text) {
    int visual_width = 0;
    for (int i = 0; text[i] != '\0'; ) {
        unsigned char c = (unsigned char)text[i];
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
            i += 1;
        }
    }
    return visual_width;
}

void draw_hp_bar(const char* label, int current, int max, int width, const char* default_color) {
    if (current < 0) current = 0;
    float percent = (float)current / max;
    int filled_len = (int)(percent * width);
    
    // 체력 비율에 따른 색상 결정 (20% 이하 빨강, 50% 이하 노랑, 그 외 초록)
    const char* bar_color = GREEN;
    if (percent <= 0.2f) bar_color = RED;
    else if (percent <= 0.5f) bar_color = YELLOW;

    // 라벨을 10칸 고정 너비로 출력 (왼쪽 정렬)
    printf("%s%-10s" RESET " [", default_color, label);

    for (int i = 0; i < width; i++) {
        if (i < filled_len) printf("%s█" RESET, bar_color);
        else printf("░");
    }
    printf("] %d/%d (%d%%)\n", current, max, (int)(percent * 100));
}

void draw_exp_bar(int current_exp, int required_exp, int width) {
    float percent = (float)current_exp / required_exp;
    if (percent > 1.0f) percent = 1.0f;
    int filled_len = (int)(percent * width);

    // HP바와 맞추기 위해 10칸 고정 너비 사용
    printf(MAGENTA "%-10s" RESET " [", "EXP");
    for (int i = 0; i < width; i++) {
        if (i < filled_len) printf(MAGENTA "█" RESET);
        else printf("░");
    }
    printf("] %.1f%% (%d/%d)\n", percent * 100, current_exp, required_exp);
}

void print_centered(const char* text, int width) {
    int v_width = get_visual_width(text);
    int padding = (width - v_width) / 2;
    if (padding < 0) padding = 0;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", text);
}

void print_divider(int width, const char* color) {
    if (color != NULL) printf("%s", color);
    // 너비를 꽉 채우기 위해 1칸짜리 '═' 또는 '━'를 width만큼 반복
    // 터미널 환경(CMD)에서 '━'가 1칸인 경우가 많으므로 width만큼 루프
    for (int i = 0; i < width; i++) printf("━"); 
    if (color != NULL) printf(RESET "\n");
    else printf("\n");
}

void print_box_line(const char* text, int width, const char* color) {
    int v_width = get_visual_width(text);
    int padding = width - v_width - 4; // 테두리 '|  ' 와 '  |' 제외
    if (padding < 0) padding = 0;

    if (color != NULL) printf("%s", color);
    printf("┃  " RESET "%s", text);
    for (int i = 0; i < padding; i++) printf(" ");
    if (color != NULL) printf("%s", color);
    printf("  ┃\n" RESET);
}

void show_title_screen() {
    clear_screen();
    int term_width = 80; // 80칸으로 표준화
    printf("\n\n");
    printf(YELLOW BOLD);
    // ASCII 아트의 길이를 term_width에 맞춰 재조정
    print_centered("  _______  __    _  _______  ___   _______  ______   _______  _______  _______", term_width);
    print_centered(" |   _   ||  |  | ||       ||   | |       ||   _  | |   _   ||       ||       |", term_width);
    print_centered(" |  |_|  ||   |_| ||_     _||   | |    ___||  | |  ||  |_|  ||_     _||    ___|", term_width);
    print_centered(" |       ||       |  |   |  |   | |   | __ |  |_|  ||       |  |   |  |   |___ ", term_width);
    print_centered(" |       ||  _    |  |   |  |   | |   ||  ||      _||       |  |   |  |    ___|", term_width);
    print_centered(" |   _   || | |   |  |   |  |   | |   |_| ||  | |  ||   _   |  |   |  |   |___ ", term_width);
    print_centered(" |__| |__||_|  |__|  |___|  |___| |_______||__| |__||__| |__|  |___|  |_______|", term_width);
    printf(RESET);
    
    printf("\n\n");
    print_divider(term_width, CYAN);
    printf(WHITE BOLD);
    print_centered("Welcome to the World of Anti-Adventure", term_width);
    printf(RESET);
    print_divider(term_width, CYAN);
    
    printf("\n\n");
    printf(CYAN);
    print_centered("- Explore Dungeons, Defeat Monsters -", term_width);
    print_centered("- Craft Legendary Gear, Master Powerful Skills -", term_width);
    printf(RESET);

    printf("\n\n\n");
    printf(BOLD YELLOW);
    print_centered(">> PRESS ENTER TO START YOUR LEGACY <<", term_width);
    printf(RESET);
    
    printf("\n\n");
    print_divider(term_width, RED);
    printf(WHITE);
    print_centered("(C) 2026 Anti-Adventure Team. All rights reserved.", term_width);
    printf(RESET);
    
    getchar();
}
