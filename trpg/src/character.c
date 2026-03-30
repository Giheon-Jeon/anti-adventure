#include <stdio.h>
#include <stdlib.h>
#include "../include/character.h"

void init_player(Player* p) {
    printf("--- 캐릭터 생성 ---\n");
    printf("이름을 입력하세요: ");
    scanf("%s", p->name);
    
    p->job = JOB_NONE;
    p->level = 1;
    p->hp = 100;
    p->max_hp = 100;
    p->mp = 50;
    p->max_mp = 50;
    
    p->str = 10;
    p->dex = 10;
    p->intel = 10;
    p->luk = 10;
    
    p->magic_atk = 0;
    p->ied = 0.0f;
    p->boss_dmg = 0.0f;
    p->dmg_percent = 0.0f;
    
    p->exp = 0;
    p->gold = 100;
    p->stat_points = 0;
    
    p->weapon_tier = 0;
    p->armor_tier = 0;
    p->accessory_tier = 0;
    
    printf("\n'%s'님, 모험을 시작합니다!\n", p->name);
}

void distribute_stats(Player* p) {
    int choice;
    while (p->stat_points > 0) {
        printf("\n====== [능력치 분배] ======\n");
        printf("남은 포인트: %d\n", p->stat_points);
        printf("1. STR (+힘): %d\n", p->str);
        printf("2. DEX (+민첩): %d\n", p->dex);
        printf("3. INT (+지능): %d\n", p->intel);
        printf("4. LUK (+운): %d\n", p->luk);
        printf("0. 나가기\n");
        printf("선택: ");
        scanf("%d", &choice);
        
        if (choice == 0) break;
        
        int mount;
        printf("투자할 포인트 양 (현재 %d 보유): ", p->stat_points);
        scanf("%d", &mount);
        
        if (mount > p->stat_points) mount = p->stat_points;
        if (mount < 0) mount = 0;
        
        switch (choice) {
            case 1: p->str += mount; break;
            case 2: p->dex += mount; break;
            case 3: p->intel += mount; break;
            case 4: p->luk += mount; break;
            default: printf("잘못된 선택입니다.\n"); continue;
        }
        p->stat_points -= mount;
    }
}

void check_level_up(Player* p) {
    int required_exp = p->level * 100;
    if (p->exp >= required_exp) {
        p->level++;
        p->exp -= required_exp;
        
        // 자동 스탯 상승 (HP/MP)
        p->max_hp += 20;
        p->hp = p->max_hp;
        p->max_mp += 10;
        p->mp = p->max_mp;
        
        // 포인트 부여
        p->stat_points += 5;
        
        printf("\n🎉 레벨 업! [Level %d]가 되었습니다! 🎉\n", p->level);
        printf("체력과 마나가 회복되었습니다.\n");

        // 1. 전직 체크 (JOB_ADVANCEMENT_LEVEL 도달 시)
        if (p->job == JOB_NONE && p->level >= JOB_ADVANCEMENT_LEVEL) {
            printf("\n✨ 새로운 힘에 눈을 뜰 때가 되었습니다... ✨\n");
            
            // 가장 높은 스탯 확인
            int max_val = p->str;
            p->job = JOB_WARRIOR;
            
            if (p->dex > max_val) { max_val = p->dex; p->job = JOB_ARCHER; }
            if (p->intel > max_val) { max_val = p->intel; p->job = JOB_MAGE; }
            if (p->luk > max_val) { max_val = p->luk; p->job = JOB_THIEF; }

            const char* job_names[] = {"없음", "전사", "궁수", "마법사", "도적"};
            printf("가장 높은 스탯에 따라 [ %s ](으)로 전직하였습니다!\n", job_names[p->job]);
            printf("이제부터 레벨업 시 포인트가 해당 주스탯에 자동 투자됩니다.\n");
        }

        // 2. 스탯 투자
        if (p->job == JOB_NONE) {
            printf("스탯 포인트 5를 획득했습니다.\n");
            distribute_stats(p);
        } else {
            // 자동 투자
            int mount = p->stat_points;
            switch (p->job) {
                case JOB_WARRIOR: p->str += mount; printf("STR이 자동으로 %d 상승했습니다!\n", mount); break;
                case JOB_ARCHER: p->dex += mount; printf("DEX가 자동으로 %d 상승했습니다!\n", mount); break;
                case JOB_MAGE: p->intel += mount; printf("INT가 자동으로 %d 상승했습니다!\n", mount); break;
                case JOB_THIEF: p->luk += mount; printf("LUK이 자동으로 %d 상승했습니다!\n", mount); break;
                default: break;
            }
            p->stat_points = 0;
        }
        
        check_level_up(p); 
    }
}

void show_status(const Player* p) {
    const char* job_names[] = {"초보자", "전사", "궁수", "마법사", "도적"};
    printf("\n====== [%s] 상태 (Level %d / %s) ======\n", p->name, p->level, job_names[p->job]);
    printf("HP: %d / %d | MP: %d / %d\n", p->hp, p->max_hp, p->mp, p->max_mp);
    printf("--------------------------\n");
    printf("STR: %d | DEX: %d\n", p->str, p->dex);
    printf("INT: %d | LUK: %d\n", p->intel, p->luk);
    printf("--------------------------\n");
    printf("마력: %d | 방무: %.1f%%\n", p->magic_atk, p->ied * 100.0f);
    printf("보뎀: %.1f%% | 뎀퍼: %.1f%%\n", p->boss_dmg * 100.0f, p->dmg_percent * 100.0f);
    printf("--------------------------\n");
    printf("EXP: %d / %d | Gold: %d\n", p->exp, p->level * 100, p->gold);
    if (p->stat_points > 0) printf(">> 사용 가능한 스탯 포인트: %d\n", p->stat_points);
    printf("========================\n");
}

void show_compact_status(const Player* p) {
    const char* job_names[] = {"초보자", "전사", "궁수", "마법사", "도적"};
    printf("\n[ %s | %s LV %d | HP: %d/%d | MP: %d/%d | Gold: %d G ]\n", 
           p->name, job_names[p->job], p->level, p->hp, p->max_hp, p->mp, p->max_mp, p->gold);
    printf("[ STR:%d DEX:%d INT:%d LUK:%d | 방무:%.0f%% 보뎀:%.0f%% ]\n", 
           p->str, p->dex, p->intel, p->luk, p->ied * 100.0f, p->boss_dmg * 100.0f);
    printf("------------------------------------------------------------\n");
}
void apply_death_penalty(Player* p) {
    printf("\n💀 [사망] 전투에서 패배했습니다! 💀\n");
    printf("강력한 사망 패널티가 적용됩니다.\n");

    // 1. 골드 감소 (50%)
    int gold_loss = p->gold / 2;
    p->gold -= gold_loss;
    printf("- 소지 골드 절반 소실: -%d G\n", gold_loss);

    // 2. 경험치 초기화 (현재 레벨의 0%)
    p->exp = 0;
    printf("- 현재 레벨 경험치 초기화: 0%%\n");

    // 3. 장비 등급 하락 (각 20% 확률)
    if (p->weapon_tier > 0 && (rand() % 100 < 20)) {
        p->weapon_tier--;
        printf("- [경고] 무기가 손상되어 등급이 하락했습니다! (Tier %d)\n", p->weapon_tier);
        // 실제 스탯 차감을 위해 장비 효과는 상점에서 다시 사거나...
        // 여기서는 그냥 티어만 낮춤 (상점 로직상 티어가 낮으면 스탯이 낮음)
    }
    if (p->armor_tier > 0 && (rand() % 100 < 20)) {
        p->armor_tier--;
        printf("- [경고] 방어구가 파손되어 등급이 하락했습니다! (Tier %d)\n", p->armor_tier);
    }
    if (p->accessory_tier > 0 && (rand() % 100 < 20)) {
        p->accessory_tier--;
        printf("- [경고] 장신구가 빛을 잃어 등급이 하락했습니다! (Tier %d)\n", p->accessory_tier);
    }

    // 4. HP/MP 복구 (최소한의 생존)
    p->hp = p->max_hp / 10;
    if (p->hp < 1) p->hp = 1;
    p->mp = p->max_mp / 10;

    printf("\n겨우 목숨만 건져 마을로 돌아왔습니다...\n");
}
