#ifndef CHARACTER_H
#define CHARACTER_H

typedef struct {
    char name[50];
    int level;
    int hp;
    int max_hp;
    int atk;
    int base_dmg;      // 기본 데미지
    float crit_chance; // 치명타 확률 (0.0 ~ 1.0)
    float crit_dmg;    // 치명타 데미지 배수 (예: 1.5)
    float dice_multiplier; // 주사위 능력 배수
    int exp;
    int gold;
} Player;

// 플레이어 초기화 함수
void init_player(Player* p);

// 플레이어 상태 출력 함수
void show_status(const Player* p);

#endif
