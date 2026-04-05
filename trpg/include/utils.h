#ifndef UTILS_H
#define UTILS_H

// ANSI Colors for GUI-like CLI
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// 난수 생성 함수 (min ~ max 사이의 정수 반환)
int get_random_range(int min, int max);

// 화면 클리어 함수
void clear_screen();

// 입력 버퍼 비우기 (scanf 머리말 처리)
void clear_input_buffer();

// 엔터 입력 대기 (일명 Pause)
void wait_for_enter();

// --- 추가된 UI 함수 ---

// HP 바 그리기 (width 만큼의 너비 사용)
void draw_hp_bar(const char* label, int current, int max, int width, const char* color);

// 텍스트 중앙 정렬 출력
void print_centered(const char* text, int width);

// 가로 구분선 출력
void print_divider(int width, const char* color);

#endif
