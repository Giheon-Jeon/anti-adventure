#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/combat.h"
#include "../include/utils.h"
#include "../include/generated_data.h"

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

// 주사위를 시각적으로 출력
void print_dice_visual(int* dice, int count) {
    printf("  ");
    for (int i = 0; i < count; i++) printf("+-------+ ");
    printf("\n");

    printf("  ");
    for (int i = 0; i < count; i++) printf("|   %d   | ", dice[i]);
    printf("\n");

    printf("  ");
    for (int i = 0; i < count; i++) printf("+-------+ ");
    printf("\n");
}

static void print_combo_effect(const char* skill_name) {
    printf("\n" MAGENTA BOLD);
    printf("  ╔════════════════════════════════════════════════════════╗\n");
    printf("  ║                                                        ║\n");
    printf("  ║         ✨ 아티팩트 콤보 스킬 발동!! ✨               ║\n");
    printf("  ║                                                        ║\n");
    printf("  ║            >> %-20s <<             ║\n");
    printf("  ║                                                        ║\n");
    printf("  ╚════════════════════════════════════════════════════════╝\n");
    printf(RESET "\n");
}

int is_combo_matched(Player* p, int dice[5], const char* skill_name) {
    int sum = 0;
    int prime_count = 0;
    int high_count = 0;
    int polar_count = 0;

    for (int i = 0; i < 5; i++) {
        sum += dice[i];
        if (dice[i] >= 4) high_count++;
        if (dice[i] == 2 || dice[i] == 3 || dice[i] == 5) prime_count++;
        if (dice[i] == 1 || dice[i] == 6) polar_count++;
    }

    if (strcmp(skill_name, "[진·용참선]") == 0) return (high_count == 5);
    else if (strcmp(skill_name, "[신·천궁살]") == 0) return (sum == 15 || sum == 20);
    else if (strcmp(skill_name, "[극·명왕권]") == 0) return (prime_count == 5);
    else if (strcmp(skill_name, "[비·영격참]") == 0) return (polar_count == 5);
    return 0;
}

// 야추 규칙 데미지 계산 (밸런스 조정 버전)
int calculate_yacht_damage(Player* p, int dice[5]) {
    int counts[7] = {0};
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        counts[dice[i]]++;
        sum += dice[i];
    }

    // 콤보 스킬 체크
    for (int i = 0; i < p->skill_count; i++) {
        if (p->learned_skills[i].type == SKILL_TYPE_COMBO) {
            if (is_combo_matched(p, dice, p->learned_skills[i].name)) {
                print_combo_effect(p->learned_skills[i].name);
                return sum * 10 * (int)p->learned_skills[i].multiplier;
            }
        }
    }

    sort_dice(dice, 5);

    // 1. Yacht (5개 동일) - 배율 대폭 하향 (안정화)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 5) {
            int mult = 50;
            const char* skill_name = "Yacht!";
            
            if (p->job == JOB_THIEF) { mult = 150; skill_name = "암영의 만찬"; }
            else if (p->job == JOB_BERSERKER) { mult = 100; skill_name = "광포한 난타"; }
            else if (p->job == JOB_ASSASSIN) { mult = 120; skill_name = "일격필살"; }
            else if (p->job == JOB_JUDGE) { mult = 130; skill_name = "최후의 심판"; }
            else if (p->job == JOB_AVATAR) { mult = 150; skill_name = "신의 집행"; }
            
            printf(YELLOW BOLD "[조합: %s (%d배)]\n" RESET, skill_name, mult);
            return sum * 10 * mult;
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
        int mult = 30;
        const char* skill_name = "Large Straight!";
        if (p->job == JOB_MAGE) { mult = 80; skill_name = "메테오 스트라이크"; }
        else if (p->job == JOB_RANGER) { mult = 60; skill_name = "폭풍의 화살"; }
        else if (p->job == JOB_GRANDMAGE) { mult = 100; skill_name = "극의의 마법"; }
        else if (p->job == JOB_CHAMPION) { mult = 70; skill_name = "섬광의 일격"; }
        else if (p->job == JOB_AVATAR) { mult = 90; skill_name = "천상낙하"; }
        
        printf(CYAN BOLD "[조합: %s (%d배)]\n" RESET, skill_name, mult);
        return sum * 10 * mult;
    }

    // 3. Full House (3+2개 동일)
    int has_3 = 0, has_2 = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) has_3 = i;
        if (counts[i] == 2) has_2 = i;
    }
    if (has_3 && has_2) {
        int mult = 25;
        const char* skill_name = "Full House!";
        if (p->job == JOB_CRUSADER) { mult = 60; skill_name = "강화된 명동"; }
        else if (p->job == JOB_PALADIN) { mult = 80; skill_name = "파쇄의 일격"; }
        
        printf(YELLOW "[조합: %s (%d배)]\n" RESET, skill_name, mult);
        return sum * 10 * mult;
    }

    // 4. Small Straight (4개 연속)
    int consecutive = 0, max_consecutive = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] > 0) consecutive++;
        else consecutive = 0;
        if (consecutive > max_consecutive) max_consecutive = consecutive;
    }
    if (max_consecutive >= 4) {
        int mult = 20;
        const char* skill_name = "Small Straight!";
        if (p->job == JOB_ARCHER) { mult = 35; skill_name = "트리플 에로우"; }
        else if (p->job == JOB_RANGER) { mult = 40; skill_name = "속사포"; }
        
        printf(CYAN "[조합: %s (%d배)]\n" RESET, skill_name, mult);
        return sum * 10 * mult;
    }

    // 5. Quad (4개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 4) {
            int mult = 15;
            printf(WHITE BOLD "[조합: Quad! (%d배)]\n" RESET, mult);
            return (i * 4 * 10 * mult) + ((sum - i * 4) * 10);
        }
    }

    // 6. Triple (3개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) {
            int mult = 10;
            printf(WHITE "[조합: Triple! (%d배)]\n" RESET, mult);
            return (i * 3 * 10 * mult) + ((sum - i * 3) * 10);
        }
    }

    // 7. Pair (2개 동일)
    for (int i = 6; i >= 1; i--) {
        if (counts[i] == 2) {
            int mult = 5;
            printf(WHITE "[조합: One Pair! (%d배)]\n" RESET, mult);
            return (i * 2 * 10 * mult) + ((sum - i * 2) * 10);
        }
    }

    printf("[조합: No Set (기본)]\n");
    return sum * 10;
}

int calculate_final_damage(Player* p, Monster* m, int yacht_result) {
    float base_stat_power = 0;
    switch (p->job) {
        case JOB_WARRIOR: base_stat_power = (p->str * 5.0f); break;
        case JOB_ARCHER:  base_stat_power = (p->dex * 5.0f); break;
        case JOB_MAGE:    base_stat_power = (p->intel * 5.0f); break;
        case JOB_THIEF:   base_stat_power = (p->luk * 5.0f); break;
        case JOB_GLADIATOR: base_stat_power = (p->str * 3.0f + p->dex * 3.0f); break;
        default:          base_stat_power = (p->str * 4.0f + p->dex); break;
    }
    
    float skill_extra_mult = 0.0f;
    int skill_extra_atk = 0;
    for (int i = 0; i < p->skill_count; i++) {
        skill_extra_mult += p->learned_skills[i].multiplier * p->learned_skills[i].level;
        skill_extra_atk += p->learned_skills[i].base_atk_bonus * p->learned_skills[i].level;
    }

    float final_skill_mult = (yacht_result / 100.0f) + skill_extra_mult; 
    float damage = (base_stat_power + skill_extra_atk) * final_skill_mult;
    damage *= (1.0f + p->dmg_percent);
    
    // 방어력 공식 변경: 단순 뺄셈에서 퍼센트 감소 방식으로 전환
    float effective_def = m->def * (1.0f - p->ied);
    if (effective_def < 0) effective_def = 0;
    
    // 데미지 감소율 산출 (방어력 100당 데미지가 절반으로 체감됨)
    float damage_reduction_multiplier = 100.0f / (100.0f + effective_def);
    int final_dmg = (int)(damage * damage_reduction_multiplier);
    
    return (final_dmg < 1) ? 1 : final_dmg;
}


void start_combat(Player* p, Dungeon* d) {
    Monster enemy;
    int roll = rand() % 100;
    int enemy_rank = 0;

    if (d->has_boss && roll >= 80) enemy = d->boss;
    else enemy = d->monsters[rand() % d->monster_count];
    
    if (d->min_cp < 500) enemy_rank = 0;
    else if (d->min_cp < 2000) enemy_rank = 1;
    else enemy_rank = 2;

    enemy.hp = enemy.max_hp;
    clear_screen();
    printf("\n" RED BOLD "--- 전투 시작: %s ---\n" RESET, enemy.name);

    while (p->hp > 0 && enemy.hp > 0) {
        printf("\nPlayer HP: %d/%d | %s HP: %d/%d\n", p->hp, p->max_hp, enemy.name, enemy.hp, enemy.max_hp);
        printf("1. 공격  2. 도망\n선택: ");
        int choice; scanf("%d", &choice);
        if (choice == 2) return;

        int p_dice[5];
        roll_dice(p_dice, 5);
        print_dice_visual(p_dice, 5);
        
        int yacht_score = calculate_yacht_damage(p, p_dice);
        int final_dmg = calculate_final_damage(p, &enemy, yacht_score);
        
        printf(YELLOW ">> 데미지: %d\n" RESET, final_dmg);
        enemy.hp -= final_dmg;

        if (enemy.hp <= 0) {
            int exp_gain = (enemy_rank + 1) * 100;
            p->exp += exp_gain;
            printf(GREEN "승리! EXP +%d 획득\n" RESET, exp_gain);
            check_level_up(p);
            return;
        }

        // 적 공격
        int e_dmg = (rand() % 10 + 5) * (enemy_rank + 1);
        printf(RED "<< 적의 공격: %d 데미지\n" RESET, e_dmg);
        p->hp -= e_dmg;
    }
}
