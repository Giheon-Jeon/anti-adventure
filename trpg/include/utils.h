#ifndef UTILS_H
#define UTILS_H

// 난수 생성 함수 (min ~ max 사이의 정수 반환)
int get_random_range(int min, int max);

// 화면 클리어 함수
void clear_screen();

// 입력 버퍼 비우기 (scanf 머리말 처리)
void clear_input_buffer();

// 엔터 입력 대기 (일명 Pause)
void wait_for_enter();

#endif
