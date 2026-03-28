#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/combat.h"
#include "../include/utils.h"

// 주사위 정렬 (오름차순)
void sort_dice(int* dice, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (dice[i] > dice[j]) {
                int temp = dice[i];
                dice[i] = dice[j];
                dice[j] = temp;
            }
        }
    }
}

// n개의 주사위 굴리기
void roll_dice(int* dice, int count) {
    for (int i = 0; i < count; i++) {
        dice[i] = (rand() % 6) + 1;
    }
}

// 야추 규칙 데미지 계산
int calculate_yacht_damage(int dice[5]) {
    int counts[7] = {0}; // 1~6 눈금 개수
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        counts[dice[i]]++;
        sum += dice[i];
    }

    sort_dice(dice, 5);

    // 1. Yacht (5개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 5) {
            printf("[조합: Yacht! (30배)]\n");
            return sum * 10 * 30;
        }
    }

    // 2. Large Straight (5개 연속)
    int is_large_straight = 1;
    for (int i = 0; i < 4; i++) {
        if (dice[i+1] != dice[i] + 1) {
            is_large_straight = 0;
            break;
        }
    }
    if (is_large_straight) {
        printf("[조합: Large Straight! (25배)]\n");
        return sum * 10 * 25;
    }

    // 3. Full House (3개 + 2개)
    int has_3 = 0, has_2 = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) has_3 = i;
        if (counts[i] == 2) has_2 = i;
    }
    if (has_3 && has_2) {
        printf("[조합: Full House! (20배)]\n");
        return sum * 10 * 20;
    }

    // 4. Small Straight (4개 연속)
    int consecutive = 0, max_consecutive = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] > 0) consecutive++;
        else consecutive = 0;
        if (consecutive > max_consecutive) max_consecutive = consecutive;
    }
    if (max_consecutive >= 4) {
        printf("[조합: Small Straight! (15배)]\n");
        return sum * 10 * 15;
    }

    // 5. Quad (4개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 4) {
            printf("[조합: Quad! (10배)]\n");
            return (i * 4 * 10 * 10) + ((sum - i * 4) * 10);
        }
    }

    // 6. Triple (3개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) {
            printf("[조합: Triple! (5배)]\n");
            return (i * 3 * 10 * 5) + ((sum - i * 3) * 10);
        }
    }

    // 7. Pair (2개 동일)
    for (int i = 6; i >= 1; i--) {
        if (counts[i] == 2) {
            printf("[조합: One Pair! (2배)]\n");
            return (i * 2 * 10 * 2) + ((sum - i * 2) * 10);
        }
    }

    // 8. No Set (기본)
    printf("[조합: No Set (기본)]\n");
    return sum * 10;
}

int calculate_final_damage(Player* p, Enemy* e, int yacht_result) {
    // 1. 기본 공격력 연산 (STR 주스탯, DEX 부스탯 가정)
    float base_stat_power = (p->str * 4.0f + p->dex);
    
    // 2. 야추 조합 배율 (yacht_result는 이미 점수화됨, 이를 %로 간주)
    float skill_mult = yacht_result / 100.0f; 
    
    float damage = base_stat_power * skill_mult;
    
    // 3. 데미지 % 적용
    damage *= (1.0f + p->dmg_percent);
    
    // 4. 보스 공격 시 추가 데미지
    if (e->is_boss) {
        damage *= (1.0f + p->boss_dmg);
    }
    
    // 5. 방어율 무시 (IED) 적용
    float effective_def = e->def * (1.0f - p->ied);
    if (effective_def < 0) effective_def = 0;
    
    int final_dmg = (int)(damage - effective_def);
    return (final_dmg < 1) ? 1 : final_dmg;
}

void start_combat(Player* p) {
    Enemy enemy;
    int enemy_type = rand() % 4; // 보스 확률 포함 (0, 1, 2 = 일반, 3 = 보스)
    
    // [Balanced] 레벨 5 미만인 경우 보스 대신 일반 몬스터 등장
    if (enemy_type == 3 && p->level < 5) {
        enemy_type = rand() % 3;
    }
    
    clear_screen();
    show_compact_status(p);
    
    if (enemy_type == 0) {
        strcpy(enemy.name, "슬라임");
        enemy.max_hp = 500;
        enemy.def = 10;
        enemy.is_boss = 0;
    } else if (enemy_type == 1) {
        strcpy(enemy.name, "고블린");
        enemy.max_hp = 1200;
        enemy.def = 30;
        enemy.is_boss = 0;
    } else if (enemy_type == 2) {
        strcpy(enemy.name, "오크");
        enemy.max_hp = 4000;
        enemy.def = 80;
        enemy.is_boss = 0;
    } else {
        strcpy(enemy.name, "🔥 자쿰의 팔 (BOSS) 🔥");
        enemy.max_hp = 15000;
        enemy.def = 200; // 높은 방어력
        enemy.is_boss = 1;
    }
    enemy.hp = enemy.max_hp;

    printf("\n--- %s(이)가 나타났다! (HP: %d, 방어율: %d) ---\n", enemy.name, enemy.hp, enemy.def);

    while (p->hp > 0 && enemy.hp > 0) {
        clear_screen();
        show_compact_status(p);
        printf("\n[%s HP: %d / %d]\n", enemy.name, enemy.hp, enemy.max_hp);
        printf("\n[플레이어 턴] Enter를 누르면 주사위를 던집니다...\n");
        clear_input_buffer(); 

        int p_dice[5];
        roll_dice(p_dice, 5);
        printf("> 주사위 결과: [%d, %d, %d, %d, %d]\n", p_dice[0], p_dice[1], p_dice[2], p_dice[3], p_dice[4]);
        
        int yacht_score = calculate_yacht_damage(p_dice);
        int final_dmg = calculate_final_damage(p, &enemy, yacht_score);
        
        printf("> 데미지 계산: (스탯 %d * 배율 %.1fx) - 방어 %+d = [ %d ]\n", 
               (int)(p->str * 4 + p->dex), yacht_score / 100.0f, (int)(enemy.def * (1.0f - p->ied)), final_dmg);
        
        enemy.hp -= final_dmg;
        if (enemy.hp < 0) enemy.hp = 0;
        printf("> %s의 HP: %d / %d\n", enemy.name, enemy.hp, enemy.max_hp);

        if (enemy.hp <= 0) {
            printf("\n%s를 처치했습니다!\n", enemy.name);
            int exp_gain = (enemy_type + 1) * 70;
            int gold_gain = (enemy_type + 1) * 150;
            if (enemy.is_boss) {
                exp_gain *= 2; gold_gain *= 5;
                printf("🏆 보스 처치 특별 보상! 🏆\n");
            }
            p->exp += exp_gain;
            p->gold += gold_gain;
            printf("EXP +%d, Gold +%d 획득!\n", exp_gain, gold_gain);
            check_level_up(p);
            break;
        }

        // 적 턴 (단순화 유지)
        printf("\n[%s의 턴] 공격합니다!\n", enemy.name);
        int e_dice[3];
        roll_dice(e_dice, 3);
        int e_dmg = (e_dice[0] + e_dice[1] + e_dice[2]) * (enemy_type + 1);
        printf("> %d의 데미지를 입었습니다!\n", e_dmg);
        p->hp -= e_dmg;
        if (p->hp < 0) p->hp = 0;
        printf("> 플레이어의 HP: %d / %d\n", p->hp, p->max_hp);

        if (p->hp <= 0) {
            apply_death_penalty(p);
            break;
        }

        wait_for_enter();
    }
}
