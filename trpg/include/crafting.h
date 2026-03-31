#ifndef CRAFTING_H
#define CRAFTING_H

#include "character.h"

// 장비 종류
#define CRAFT_TYPE_WEAPON 1
#define CRAFT_TYPE_ARMOR 2
#define CRAFT_TYPE_ACCESSORY 3

typedef struct {
    char material_name[50];
    int count;
} Material;

typedef struct {
    int target_tier;
    int craft_type; // 무기, 방어구, 장신구
    Material materials[3];
    int material_count;
    int success_rate; // 0 ~ 100
    int failure_loss_rate; // 실패 시 재료 소실 확률 (%)
} Recipe;

// 대장간 메뉴 실행
void open_crafting_menu(Player* p);

#endif
