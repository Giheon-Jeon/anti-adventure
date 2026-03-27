#ifndef COMBAT_H
#define COMBAT_H

#include "character.h"

typedef struct {
    char name[50];
    int hp;
    int max_hp;
} Enemy;

// 주사위 굴리기 (n개)
void roll_dice(int* dice, int count);

// 야추 규칙 데미지 계산 (5개 주사위)
int calculate_yacht_damage(int dice[5]);

// 전투 시작 루프
void start_combat(Player* p);

#endif
