#ifndef CHARACTER_H
#define CHARACTER_H

typedef struct {
    char name[50];
    int level;
    int hp;
    int max_hp;
    int atk;
    int exp;
    int gold;
} Player;

// 플레이어 초기화 함수
void init_player(Player* p);

// 플레이어 상태 출력 함수
void show_status(const Player* p);

#endif
