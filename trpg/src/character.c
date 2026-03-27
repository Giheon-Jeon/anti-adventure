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
    p->def = 5;      // 기본 방어력
    p->base_dmg = 5;
    p->crit_chance = 0.05f; // 5%
    p->crit_dmg = 1.5f;     // 150%
    p->dice_multiplier = 1.0f;
    p->exp = 0;
    p->gold = 100; // 초기 골드를 조금 줍니다.
    
    p->weapon_tier = 0;
    p->armor_tier = 0;
    p->accessory_tier = 0;
    
    printf("\n'%s'님, 모험을 시작합니다!\n", p->name);
}

void check_level_up(Player* p) {
    int required_exp = p->level * 100;
    if (p->exp >= required_exp) {
        p->level++;
        p->exp -= required_exp;
        
        // 스탯 상승
        p->max_hp += 10;
        p->hp = p->max_hp; // 체력 완전 회복
        p->atk += 2;
        p->def += 1;
        
        printf("\n🎉 레벨 업! [Level %d]가 되었습니다! 🎉\n", p->level);
        printf("체력이 모두 회복되었고, 공격력/방어력이 상승했습니다.\n");
        
        // 추가 레벨업 확인 (재귀적 호출 또는 루프)
        check_level_up(p); 
    }
}

void show_status(const Player* p) {
    printf("\n====== [%s] 상태 ======\n", p->name);
    printf("Level: %d\n", p->level);
    printf("HP: %d / %d\n", p->hp, p->max_hp);
    printf("공격력: %d (+기본 %d)\n", p->atk, p->base_dmg);
    printf("방어력: %d\n", p->def);
    printf("치명타: %.1f%% (배율 %.1fx)\n", p->crit_chance * 100.0f, p->crit_dmg);
    printf("주사위 배수: %.1fx\n", p->dice_multiplier);
    printf("EXP: %d\n", p->exp);
    printf("Gold: %d\n", p->gold);
    printf("========================\n");
}
