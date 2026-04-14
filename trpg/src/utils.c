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

    int visual_width = get_visual_width(label);
    int padding = 12 - visual_width;
    if (padding < 0) padding = 0;

    printf("%s%s" RESET, default_color, label);
    for (int i = 0; i < padding; i++) printf(" ");
    printf(" [");

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

    printf(CYAN "EXP [" RESET);
    for (int i = 0; i < width; i++) {
        if (i < filled_len) printf(MAGENTA "█" RESET);
        else printf("░");
    }
    printf(CYAN "] %.1f%%" RESET " (%d/%d)\n", percent * 100, current_exp, required_exp);
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
    for (int i = 0; i < width / 2; i++) printf("━"); // '━'는 보통 2칸 차지
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
    printf("\n\n");
    printf(YELLOW BOLD);
    print_centered("   ___   _  _  _____  ___  ___   ___   _   _  ___  _  _  _____  _   _  ___   ___  ", 80);
    print_centered("  / _ \\ | \\| ||_   _||_ _|/ __| | _ \\ /_\\ | _ \\| \\| ||_   _|| | | || _ \\ | __| ", 80);
    print_centered(" |  _  || .  |  | |   | || (_ | |   // _ \\|   /| .  |  | |  | |_| ||   / | _|  ", 80);
    print_centered(" |_| |_||_|\\_|  |_|  |___|\\___| |_|_/_/ \\_\\|_|_\\|_|\\_|  |_|   \\___/ |_|_\\ |___| ", 80);
    printf(RESET);
    
    printf("\n\n");
    print_divider(80, CYAN);
    printf(WHITE BOLD);
    print_centered("Welcome to the World of Anti-Adventure", 80);
    printf(RESET);
    print_divider(80, CYAN);
    
    printf("\n\n");
    printf(CYAN);
    print_centered("- Explore Dungeons, Defeat Monsters -", 80);
    print_centered("- Craft Legendary Gear, Master Powerful Skills -", 80);
    printf(RESET);

    printf("\n\n\n");
    printf(BOLD YELLOW);
    print_centered(">> PRESS ENTER TO START YOUR LEGACY <<", 80);
    printf(RESET);
    
    printf("\n\n");
    print_divider(80, RED);
    printf(WHITE);
    print_centered("(C) 2026 Anti-Adventure Team. All rights reserved.", 80);
    printf(RESET);
    
    getchar(); // 타이틀 화면 대기
}
