#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/combat.h"

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

void start_combat(Player* p) {
    Enemy enemy;
    int enemy_type = rand() % 3;
    if (enemy_type == 0) {
        strcpy(enemy.name, "슬라임");
        enemy.max_hp = 500; // 체력 소폭 하향 조정
    } else if (enemy_type == 1) {
        strcpy(enemy.name, "고블린");
        enemy.max_hp = 1000;
    } else {
        strcpy(enemy.name, "오크");
        enemy.max_hp = 3000;
    }
    enemy.hp = enemy.max_hp;

    printf("\n--- %s(이)가 나타났다! (HP: %d) ---\n", enemy.name, enemy.hp);

    while (p->hp > 0 && enemy.hp > 0) {
        printf("\n[플레이어 턴] 주사위 5개를 굴립니다... (Enter를 누르세요)\n");
        while (getchar() != '\n'); // Enter 입력 대기

        int p_dice[5];
        roll_dice(p_dice, 5);
        printf("> 주사위 결과: [%d, %d, %d, %d, %d]\n", p_dice[0], p_dice[1], p_dice[2], p_dice[3], p_dice[4]);
        
        int p_dmg = calculate_yacht_damage(p_dice);
        printf("> 총 %d의 데미지를 입혔습니다!\n", p_dmg);
        enemy.hp -= p_dmg;
        if (enemy.hp < 0) enemy.hp = 0;
        printf("> %s의 HP: %d / %d\n", enemy.name, enemy.hp, enemy.max_hp);

        if (enemy.hp <= 0) {
            printf("\n%s를 처치했습니다!\n", enemy.name);
            p->exp += (enemy_type + 1) * 50;
            p->gold += (enemy_type + 1) * 100;
            printf("EXP +%d, Gold +%d 획득!\n", (enemy_type + 1) * 50, (enemy_type + 1) * 100);
            check_level_up(p);
            break;
        }

        // 적 턴
        printf("\n[%s의 턴] 주사위 3개를 굴려 공격합니다!\n", enemy.name);
        int e_dice[3];
        roll_dice(e_dice, 3);
        int e_sum = e_dice[0] + e_dice[1] + e_dice[2];
        int e_dmg = e_sum / 2; // 데미지 대폭 하향 (10 이내)
        printf("> 적 주사위: [%d, %d, %d] -> %d 데미지!\n", e_dice[0], e_dice[1], e_dice[2], e_dmg);
        p->hp -= e_dmg;
        if (p->hp < 0) p->hp = 0;
        printf("> 플레이어의 HP: %d / %d\n", p->hp, p->max_hp);

        if (p->hp <= 0) {
            printf("\n패배했습니다... 마을로 돌아갑니다.\n");
            p->hp = p->max_hp / 2; // 페널티
            break;
        }
    }
}
