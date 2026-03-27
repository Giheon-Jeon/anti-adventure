#include <stdio.h>
#include <stdio.h>
#include "../include/character.h"

void init_player(Player* p) {
    printf("--- 캐릭터 생성 ---\n");
    printf("이름을 입력하세요: ");
    scanf("%s", p->name);
    
    p->level = 1;
    p->hp = 100;
    p->max_hp = 100;
    p->atk = 10;
    p->base_dmg = 5;
    p->crit_chance = 0.05f; // 5%
    p->crit_dmg = 1.5f;     // 150%
    p->dice_multiplier = 1.0f;
    p->exp = 0;
    p->gold = 100; // 초기 골드를 조금 줍니다.
    
    printf("\n'%s'님, 모험을 시작합니다!\n", p->name);
}

void show_status(const Player* p) {
    printf("\n====== [%s] 상태 ======\n", p->name);
    printf("Level: %d\n", p->level);
    printf("HP: %d / %d\n", p->hp, p->max_hp);
    printf("공격력: %d (+기본 %d)\n", p->atk, p->base_dmg);
    printf("치명타: %.1f%% (배율 %.1fx)\n", p->crit_chance * 100.0f, p->crit_dmg);
    printf("주사위 배수: %.1fx\n", p->dice_multiplier);
    printf("EXP: %d\n", p->exp);
    printf("Gold: %d\n", p->gold);
    printf("========================\n");
}
