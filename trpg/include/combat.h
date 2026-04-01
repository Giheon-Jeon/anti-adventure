#ifndef COMBAT_H
#define COMBAT_H

#include "character.h"
#include "item.h"

typedef struct {
    char name[50];
    int hp;
    int max_hp;
    int def;    // 방어율
    int is_boss; // 보스 여부 (0: 일반, 1: 보스)
    Item loot;  // 전리품
} Monster;

typedef struct {
    char name[50];
    int min_cp;
    int max_cp;
    Monster monsters[3];
    int monster_count;
    Monster boss;
    int has_boss;
} Dungeon;

// 주사위 굴리기 (n개)
void roll_dice(int* dice, int count);

// 야추 규칙 데미지 계산
int calculate_yacht_damage(Player* p, int dice[5]);
int calculate_final_damage(Player* p, Monster* m, int yacht_result);

// 전투 시작 (특정 던전 내에서의 전투)
void start_combat(Player *p, Dungeon *d);

// 던전 선택 메뉴
void select_dungeon(Player* p);

#endif
