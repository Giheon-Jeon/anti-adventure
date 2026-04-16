#ifndef SAVELOAD_H
#define SAVELOAD_H

#include "character.h"

#define SAVE_DIR "saves"
#define MAX_SAVE_SLOTS 20
#define MAX_FILENAME_LEN 256

// 게임 저장 (내부용 — saves/{캐릭터이름}.csv 에 자동 저장)
int save_game(Player* p, const char* filepath);

// 게임 불러오기
int load_game(Player* p, const char* filepath);

// 즉시 저장 (캐릭터 이름 기반 파일명 자동 결정, 덮어쓰기)
void quick_save(Player* p);

// 불러오기 메뉴 UI (파일 목록에서 선택)
// 반환: 1=성공적으로 로드됨, 0=취소 또는 실패
int show_load_menu(Player* p);

#endif
