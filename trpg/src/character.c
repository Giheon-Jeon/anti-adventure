#include <stdio.h>
#include <string.h>
#include "character.h"

void init_player(Player* p) {
    printf("--- 캐릭터 생성 ---\n");
    printf("이름을 입력하세요: ");
    scanf("%s", p->name);
    
    p->level = 1;
    p->hp = 100;
    p->max_hp = 100;
    p->atk = 10;
    p->exp = 0;
    p->gold = 0;
    
    printf("\n'%s'님, 모험을 시작합니다!\n", p->name);
}

void show_status(const Player* p) {
    printf("\n====== [%s] 상태 ======\n", p->name);
    printf("Level: %d\n", p->level);
    printf("HP: %d / %d\n", p->hp, p->max_hp);
    printf("공격력: %d\n", p->atk);
    printf("EXP: %d\n", p->exp);
    printf("Gold: %d\n", p->gold);
    printf("========================\n");
}
