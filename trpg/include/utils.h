#ifndef UTILS_H
#define UTILS_H

// ANSI Colors for GUI-like CLI
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BLACK   "\033[30m"
#define RED_BG  "\033[41m"
#define GREEN_BG "\033[42m"
#define YELLOW_BG "\033[43m"
#define MAGENTA_BG "\033[45m"
#define CYAN_BG   "\033[46m"
#define BLUE_BG   "\033[44m"
#define WHITE_BG  "\033[47m"
#define WHITE     "\033[37m"


// 난수 생성 함수 (min ~ max 사이의 정수 반환)
int get_random_range(int min, int max);

// 화면 클리어 함수
void clear_screen();

// 입력 버퍼 비우기 (scanf 머리말 처리)
void clear_input_buffer();

// 엔터 입력 대기 (일명 Pause)
void wait_for_enter();

// --- 추가된 UI 함수 ---

// HP 바 그리기
void draw_hp_bar(const char* label, int current, int max, int width, const char* color);

// EXP 바 그리기
void draw_exp_bar(int current_exp, int required_exp, int width);

// 텍스트의 시각적 너비 계산
int get_visual_width(const char* text);

// 텍스트 중앙 정렬 출력
void print_centered(const char* text, int width);

// 가로 구분선 출력
void print_divider(int width, const char* color);

// 상자 형태의 라인 출력 (정렬 문제 해결용)
void print_box_line(const char* text, int width, const char* color);

// 타이틀 화면 (Game Start Screen)
// 반환값: 1 = 새로 시작, 2 = 이어하기
int show_title_screen();

#endif
