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
    int is_boss = (rand() % 4 == 3); // 25% chance for boss
    int type = rand() % 3; // 0, 1, 2 sub-types
    int enemy_rank = 0; // For reward calculation

    clear_screen();
    show_compact_status(p);

    if (p->level < 5) {
        // Level 1-4: Basic
        is_boss = 0; // No boss below level 5
        enemy_rank = 0;
        if (type == 0) {
            strcpy(enemy.name, "파란 달팽이");
            enemy.max_hp = 300; enemy.def = 5;
            strcpy(enemy.loot.name, "달팽이 껍질");
            strcpy(enemy.loot.description, "작고 단단한 파란색 껍질");
            enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 10;
        } else if (type == 1) {
            strcpy(enemy.name, "슬라임");
            enemy.max_hp = 600; enemy.def = 10;
            strcpy(enemy.loot.name, "슬라임의 핵");
            strcpy(enemy.loot.description, "구슬처럼 빛나는 말랑한 핵");
            enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 20;
        } else {
            strcpy(enemy.name, "주황버섯");
            enemy.max_hp = 1000; enemy.def = 20;
            strcpy(enemy.loot.name, "버섯 갓");
            strcpy(enemy.loot.description, "주황색 무늬가 있는 버섯 갓");
            enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 30;
        }
        enemy.is_boss = 0;
    } else if (p->level < 10) {
        // Level 5-9: Intermediate
        enemy_rank = 1;
        if (is_boss) {
            strcpy(enemy.name, "🔥 자쿰의 팔 (BOSS) 🔥");
            enemy.max_hp = 25000; enemy.def = 150; enemy.is_boss = 1;
            strcpy(enemy.loot.name, "자쿰의 나뭇가지");
            strcpy(enemy.loot.description, "자쿰의 팔에서 떨어진 단단한 가지");
            enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 1000;
        } else {
            if (type == 0) {
                strcpy(enemy.name, "고블린");
                enemy.max_hp = 3000; enemy.def = 40;
                strcpy(enemy.loot.name, "고블린의 몽둥이");
                strcpy(enemy.loot.description, "투박한 나무 몽둥이");
                enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 100;
            } else if (type == 1) {
                strcpy(enemy.name, "오크");
                enemy.max_hp = 6000; enemy.def = 80;
                strcpy(enemy.loot.name, "오크의 이빨");
                strcpy(enemy.loot.description, "거칠게 갈린 오크의 송곳니");
                enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 150;
            } else {
                strcpy(enemy.name, "와일드카고");
                enemy.max_hp = 10000; enemy.def = 120;
                strcpy(enemy.loot.name, "와일드카고의 눈동자");
                strcpy(enemy.loot.description, "무섭게 빛나는 눈동자");
                enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 200;
            }
            enemy.is_boss = 0;
        }
    } else if (p->level < 15) {
        // Level 10-14: Advanced
        enemy_rank = 2;
        if (is_boss) {
            strcpy(enemy.name, "🕒 시계탑의 파풀라투스 (BOSS) 🕒");
            enemy.max_hp = 150000; enemy.def = 400; enemy.is_boss = 1;
            strcpy(enemy.loot.name, "시계 파편");
            strcpy(enemy.loot.description, "시간의 마력이 깃든 시계 부품");
            enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 5000;
        } else {
            if (type == 0) {
                strcpy(enemy.name, "스켈레톤");
                enemy.max_hp = 25000; enemy.def = 200;
                strcpy(enemy.loot.name, "부서진 뼈");
                strcpy(enemy.loot.description, "말라비틀어진 창백한 뼈");
                enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 500;
            } else if (type == 1) {
                strcpy(enemy.name, "다크 레인저");
                enemy.max_hp = 45000; enemy.def = 300;
                strcpy(enemy.loot.name, "검은 화살촉");
                strcpy(enemy.loot.description, "어둠의 기운이 감도는 화살촉");
                enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 800;
            } else {
                strcpy(enemy.name, "듀얼 파이렛");
                enemy.max_hp = 70000; enemy.def = 350;
                strcpy(enemy.loot.name, "해적의 동전");
                strcpy(enemy.loot.description, "해적들이 사용하던 낡은 금화");
                enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 1000;
            }
            enemy.is_boss = 0;
        }
    } else {
        // Level 15+: Elite
        enemy_rank = 3;
        if (is_boss) {
            strcpy(enemy.name, "🌸 군단장 핑크빈 (BOSS) 🌸");
            enemy.max_hp = 800000; enemy.def = 1000; enemy.is_boss = 1;
            strcpy(enemy.loot.name, "핑크빈의 인형");
            strcpy(enemy.loot.description, "폭신폭신하고 귀여운 핑크색 인형");
            enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 20000;
        } else {
            if (type == 0) {
                strcpy(enemy.name, "블루 와이번");
                enemy.max_hp = 150000; enemy.def = 500;
                strcpy(enemy.loot.name, "와이번의 발톱");
                strcpy(enemy.loot.description, "날카로운 블루 와이번의 발톱");
                enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 2000;
            } else if (type == 1) {
                strcpy(enemy.name, "다크 코르니안");
                enemy.max_hp = 300000; enemy.def = 700;
                strcpy(enemy.loot.name, "단단한 비늘");
                strcpy(enemy.loot.description, "검은 기운이 서린 단단한 비늘");
                enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 3000;
            } else {
                strcpy(enemy.name, "스켈레곤");
                enemy.max_hp = 500000; enemy.def = 900;
                strcpy(enemy.loot.name, "용의 뼈");
                strcpy(enemy.loot.description, "거대한 용의 화석 조각");
                enemy.loot.type = ITEM_TYPE_TROPHY; enemy.loot.value = 5000;
            }
            enemy.is_boss = 0;
        }
    }
    enemy.hp = enemy.max_hp;

    printf("\n--- %s(이)가 나타났다! (HP: %d, 방어율: %d) ---\n", enemy.name, enemy.hp, enemy.def);


    while (p->hp > 0 && enemy.hp > 0) {
        clear_screen();
        show_compact_status(p);
        printf("\n[%s HP: %d / %d]\n", enemy.name, enemy.hp, enemy.max_hp);
        
        int choice;
        printf("\n[플레이어 턴]\n1. 공격하기\n2. 도망가기\n선택: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        int skip_attack = 0;
        if (choice == 2) {
            int escape_dice[5];
            roll_dice(escape_dice, 5);
            int sum = 0;
            printf("> 도망 주사위 결과: [");
            for (int i = 0; i < 5; i++) {
                sum += escape_dice[i];
                printf("%d%s", escape_dice[i], (i == 4) ? "" : ", ");
            }
            printf("] (합계: %d)\n", sum);

            if (sum >= 15) {
                printf("✨ 무사히 도망쳤습니다! ✨\n");
                wait_for_enter();
                return;
            } else {
                printf("💀 도망에 실패했습니다! 공격 기회를 잃었습니다... 💀\n");
                skip_attack = 1;
            }
        }

        if (!skip_attack) {
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
                int exp_gain = (enemy_rank + 1) * 100;
                int gold_gain = (enemy_rank + 1) * 200;
                if (enemy.is_boss) {
                    exp_gain *= 5; gold_gain *= 10;
                    printf("🏆 보스 처치 특별 보상! 🏆\n");
                }
                p->exp += exp_gain;
                p->gold += gold_gain;
                printf("EXP +%d, Gold +%d 획득!\n", exp_gain, gold_gain);
                
                // 전리품 획득 로직
                if (p->inventory.count < MAX_INVENTORY_SIZE) {
                    p->inventory.items[p->inventory.count] = enemy.loot;
                    p->inventory.count++;
                    printf("🎁 전리품 획득: [%s] 🎁\n", enemy.loot.name);
                } else {
                    printf("🎒 인벤토리가 가득 차서 [%s](을)를 얻지 못했습니다.\n", enemy.loot.name);
                }

                check_level_up(p);
                wait_for_enter();
                break;
            }
        }

        // 적 턴 (단순화 유지)
        printf("\n[%s의 턴] 공격합니다!\n", enemy.name);
        int e_dice[3];
        roll_dice(e_dice, 3);
        int e_dmg = (e_dice[0] + e_dice[1] + e_dice[2]) * (enemy_rank + 1) * 2;
        if (enemy.is_boss) e_dmg *= 3;
        printf("> %d의 데미지를 입었습니다!\n", e_dmg);
        p->hp -= e_dmg;
        if (p->hp < 0) p->hp = 0;
        printf("> 플레이어의 HP: %d / %d\n", p->hp, p->max_hp);

        if (p->hp <= 0) {
            apply_death_penalty(p);
            wait_for_enter();
            break;
        }

        wait_for_enter();
    }
}
