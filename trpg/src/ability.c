#include "../include/ability.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* get_ability_rank_name(AbilityRank r) {
    switch (r) {
        case ABILITY_RANK_NORMAL: return "NORMAL";
        case ABILITY_RANK_RARE: return "RARE";
        case ABILITY_RANK_EPIC: return "EPIC";
        case ABILITY_RANK_UNIQUE: return "UNIQUE";
        case ABILITY_RANK_LEGENDARY: return "LEGENDARY";
        default: return "UNKNOWN";
    }
}

const char* get_ability_type_name(AbilityType t) {
    switch (t) {
        case ABILITY_TYPE_STR_FLAT: return "STR 증가 (고정)";
        case ABILITY_TYPE_DEX_FLAT: return "DEX 증가 (고정)";
        case ABILITY_TYPE_INT_FLAT: return "INT 증가 (고정)";
        case ABILITY_TYPE_LUK_FLAT: return "LUK 증가 (고정)";
        case ABILITY_TYPE_STR_PER: return "STR 증가 (%)";
        case ABILITY_TYPE_DEX_PER: return "DEX 증가 (%)";
        case ABILITY_TYPE_INT_PER: return "INT 증가 (%)";
        case ABILITY_TYPE_LUK_PER: return "LUK 증가 (%)";
        case ABILITY_TYPE_BOSS_DMG: return "보스 데미지 증가";
        case ABILITY_TYPE_IED: return "방어율 무시 증가";
        case ABILITY_TYPE_DMG_PER: return "총 데미지 증가";
        case ABILITY_TYPE_DOUBLE_ATTACK: return "연속 공격 확률";
        default: return "없음";
    }
}

Ability generate_random_ability() {
    Ability a;
    int r_roll = rand() % 100;
    
    // 등급 결정
    if (r_roll < 1) a.rank = ABILITY_RANK_LEGENDARY;
    else if (r_roll < 5) a.rank = ABILITY_RANK_UNIQUE;
    else if (r_roll < 20) a.rank = ABILITY_RANK_EPIC;
    else if (r_roll < 50) a.rank = ABILITY_RANK_RARE;
    else a.rank = ABILITY_RANK_NORMAL;
    
    // 타입 결정 (등급에 따라 가능한 타입 제한)
    int type_count = 12; // 총 12가지
    int t_roll = rand() % type_count + 1;
    a.type = (AbilityType)t_roll;
    
    // 수치 결정 (등급별 차등)
    switch (a.type) {
        case ABILITY_TYPE_STR_FLAT:
        case ABILITY_TYPE_DEX_FLAT:
        case ABILITY_TYPE_INT_FLAT:
        case ABILITY_TYPE_LUK_FLAT: {
            int vals[] = {5, 15, 30, 60, 150};
            a.value = (float)vals[a.rank];
            break;
        }
        case ABILITY_TYPE_STR_PER:
        case ABILITY_TYPE_DEX_PER:
        case ABILITY_TYPE_INT_PER:
        case ABILITY_TYPE_LUK_PER: {
            float vals[] = {1, 3, 5, 10, 20};
            a.value = vals[a.rank];
            break;
        }
        case ABILITY_TYPE_BOSS_DMG:
        case ABILITY_TYPE_IED:
        case ABILITY_TYPE_DMG_PER: {
            float vals[] = {2, 5, 10, 15, 30};
            a.value = vals[a.rank];
            break;
        }
        case ABILITY_TYPE_DOUBLE_ATTACK: {
            float vals[] = {1, 5, 10, 15, 25};
            a.value = vals[a.rank];
            break;
        }
        default: a.value = 0; break;
    }
    
    return a;
}

void open_ability_menu(Player* p) {
    int choice;
    while (1) {
        clear_screen();
        show_compact_status(p);
        printf("\n========= [신비한 재단 - 어빌리티] =========\n");
        printf("캐릭터의 잠재력을 일깨워 강력한 능력을 얻습니다.\n\n");
        
        for (int i = 0; i < ABILITY_COUNT; i++) {
            printf("[%d번 슬롯] ", i + 1);
            if (p->abilities[i].type == ABILITY_TYPE_NONE) {
                printf("비어 있음\n");
            } else {
                const char* r_name = get_ability_rank_name(p->abilities[i].rank);
                const char* t_name = get_ability_type_name(p->abilities[i].type);
                printf("[%s] %s +%.0f%s %s\n", 
                       r_name, t_name, p->abilities[i].value, 
                       (p->abilities[i].type >= ABILITY_TYPE_STR_PER ? "%" : ""),
                       (p->ability_locked[i] ? "🔒" : "🔓"));
            }
        }
        
        int locked_count = 0;
        for (int i = 0; i < ABILITY_COUNT; i++) if (p->ability_locked[i]) locked_count++;
        
        int cost = 1000;
        if (locked_count == 1) cost = 2000;
        else if (locked_count == 2) cost = 4000;
        
        printf("\n------------------------------------------\n");
        printf("1~3. 슬롯 잠금/해제\n");
        if (locked_count < 3) {
            printf("4. 어빌리티 재설정 (%d G 소모)\n", cost);
        } else {
            printf("4. 모든 슬롯이 잠겨 재설정 불가\n");
        }
        printf("0. 나가기\n");
        printf("------------------------------------------\n");
        printf("선택: ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        if (choice == 0) break;
        
        if (choice >= 1 && choice <= 3) {
            int idx = choice - 1;
            p->ability_locked[idx] = !p->ability_locked[idx];
            printf("[알림] %d번 슬롯을 %s했습니다.\n", choice, p->ability_locked[idx] ? "잠금" : "잠금 해제");
        } else if (choice == 4) {
            if (locked_count >= 3) {
                printf("[오류] 최소 한 개 이상의 슬롯은 열려 있어야 합니다.\n");
            } else if (p->gold < cost) {
                printf("[오류] 골드가 부족합니다!\n");
            } else {
                p->gold -= cost;
                printf("\n✨ 신비한 기운이 캐릭터를 감쌉니다... ✨\n");
                for (int i = 0; i < ABILITY_COUNT; i++) {
                    if (!p->ability_locked[i]) {
                        p->abilities[i] = generate_random_ability();
                    }
                }
                update_combat_power(p);
            }
        }
        wait_for_enter();
    }
}
