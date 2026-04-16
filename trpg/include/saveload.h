#ifndef SAVELOAD_H
#define SAVELOAD_H

#include "character.h"

#define SAVE_DIR "saves"
#define MAX_SAVE_SLOTS 20
#define MAX_FILENAME_LEN 256

// 게임 저장 (CSV 섹션 포맷)
int save_game(Player* p, const char* filepath);

// 게임 불러오기
int load_game(Player* p, const char* filepath);

// 저장 메뉴 UI (파일명 지정)
void show_save_menu(Player* p);

// 불러오기 메뉴 UI (파일 목록에서 선택)
// 반환: 1=성공적으로 로드됨, 0=취소 또는 실패
int show_load_menu(Player* p);

#endif
