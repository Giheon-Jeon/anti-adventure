#include <stdio.h>
#include <stdlib.h>
#include "../include/event.h"
#include "../include/combat.h"
#include "../include/utils.h"

static int check_stat(Player* p, const char* stat_name, int p_stat, int threshold) {
    printf("\n[%s 체크] (필요: %d / 현재: %d)\n", stat_name, threshold, p_stat);
    int roll = (rand() % 12) + 1; // 1~12 다이스 루틴으로 변경 (좀 더 안정적)
    int total = p_stat + roll;
    printf("결과: %d (다이스 %d + 능력치 %d)\n", total, roll, p_stat);
    if (total >= threshold) {
        printf("=> [성공!] 위기를 넘기고 기회를 잡았습니다.\n");
        return 1;
    } else {
        printf("=> [실패...] 운이 따르지 않았습니다.\n");
        return 0;
    }
}

// --- [RARE] 이벤트를 위한 보상형 이벤트 ---

static void event_ancient_dragon_altar(Player* p) {
    printf("\n[희귀 이벤트] 웅장한 '고대 용의 제단'을 발견했습니다.\n");
    printf("제단에 새겨진 문구: \"그릇을 비우는 자, 진정한 힘을 얻으리라.\"\n");
    printf("최대 HP의 일부를 희생하여 강력한 파괴력을 얻겠습니까?\n");
    printf("1. 희생한다 (최대 HP -50, 보스 데미지 +10%%)\n2. 거절한다\n");
    printf("선택: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) { clear_input_buffer(); return; }
    clear_input_buffer();
    
    if (choice == 1) {
        if (p->max_hp <= 60) {
            printf("생명력이 너무 부족하여 제단이 당신을 거부합니다.\n");
            return;
        }
        printf("\n피의 계약이 성사되었습니다. 온몸에 파괴적인 기운이 감돕니다!\n");
        p->max_hp -= 50;
        if (p->hp > p->max_hp) p->hp = p->max_hp;
        p->boss_dmg += 0.10f;
        printf("[영구 보너스] 최대 HP -50, 보스 데미지 +10.0%%\n");
    } else {
        printf("불안한 기운을 뒤로하고 제단을 떠납니다.\n");
    }
}

static void event_void_sage(Player* p) {
    printf("\n[희귀 이벤트] 차원의 틈새에서 '공허의 현자'를 만났습니다.\n");
    printf("현자가 당신의 정신력을 시험하려 합니다. 통과하면 심오한 지식을 나누어 준다고 합니다.\n");
    
    if (check_stat(p, "지능(INT)", p->intel, 25)) {
        printf("\n현자가 만족스러운 듯 미소 짓습니다. \"공허의 진리를 깨달았구나.\"\n");
        p->ied += 0.05f;
        printf("[영구 보너스] 방어율 무시(IED) +5.0%%\n");
        grant_event_skill(p, "보이드 스트라이크", "공허의 힘을 빌려 적의 방어를 꿰뚫습니다.", 2.5f, 50, 0, 0, 0, 0, 0, 20, 40);
    } else {
        printf("\n현자가 고개를 저으며 안개 속으로 사라집니다. \"아직 때가 아니로군.\"\n");
    }
}

// --- [UNCOMMON] 이벤트 ---

static void event_cursed_chest(Player* p) {
    printf("\n[이벤트] 불길한 보랏빛 기운이 감도는 '저주받은 상자'를 발견했습니다.\n");
    printf("운명을 시험하시겠습니까? (성공 시 전설적 전리품, 실패 시 심각한 부상)\n");
    printf("1. 연다\n2. 지나친다\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) { clear_input_buffer(); return; }
    clear_input_buffer();
    
    if (choice == 1) {
        if (check_stat(p, "행운(LUK)", p->luk, 20)) {
            printf("\n저주를 이겨내고 상자가 열렸습니다!\n");
            printf("[보상] 황금 용의 비늘 (가치 2000G), EXP +500\n");
            p->exp += 500;
            p->gold += 2000;
            check_level_up(p);
        } else {
            printf("\n상자에서 검은 연기가 뿜어져 나와 당신을 덮칩니다!\n");
            int damage = p->hp / 2;
            p->hp -= damage;
            if (p->hp < 1) p->hp = 1;
            printf("[패널티] 현재 체력이 절반으로 감소했습니다! (HP -%d)\n", damage);
        }
    }
}

static void event_mysterious_labyrinth(Player* p) {
    printf("\n[이벤트] 신비로운 미궁의 입구에 도착했습니다.\n");
    printf("어떤 길을 선택하시겠습니까?\n");
    printf("1. 복잡한 함정 구역 (민첩 DEX 체크)\n2. 고대 문자가 새겨진 구역 (지능 INT 체크)\n");
    printf("선택: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) { clear_input_buffer(); return; }
    clear_input_buffer();
    
    if (choice == 1) {
        if (check_stat(p, "민첩(DEX)", p->dex, 18)) {
            printf("\n함정을 가볍게 피하며 미궁 깊은 곳의 비보를 찾았습니다!\n");
            printf("[보상] 민첩 포인트 상승 및 경험치 획득! (DEX +3, EXP +200)\n");
            p->dex += 3;
            p->exp += 200;
            check_level_up(p);
        } else {
            printf("\n함정에 휘말려 만신창이가 된 채 겨우 빠져나왔습니다...\n");
            p->hp -= (p->max_hp * 0.2f);
            if (p->hp < 1) p->hp = 1;
        }
    } else {
        if (check_stat(p, "지능(INT)", p->intel, 18)) {
            printf("\n문제를 풀고 미궁의 비밀을 알아냈습니다!\n");
            printf("[보상] 지능 포인트 상승 및 경험치 획득! (INT +3, EXP +200)\n");
            p->intel += 3;
            p->exp += 200;
            check_level_up(p);
        } else {
            printf("\n해답을 찾지 못해 미궁 속에서 한참을 헤매며 기력을 소진했습니다...\n");
            p->mp = 0;
            printf("[패널티] MP가 모두 소진되었습니다.\n");
        }
    }
}


static void event_mysterious_chest(Player* p) {
    printf("\n[이벤트] 낡은 보물상자를 발견했습니다!\n");
    printf("상자를 열어보기 위해 주사위 5개를 던지시겠습니까?\n");
    printf("1. 연다 (성공 시 큰 보상, 실패 시 함정 공격)\n2. 무시한다\n");
    printf("선택: ");
    
    int choice;
    scanf("%d", &choice);
    clear_input_buffer();
    
    if (choice == 1) {
        printf("\n주사위 5개를 던집니다...\n");
        int dice[5];
        roll_dice(dice, 5);
        sort_dice(dice, 5); // 주사위 정렬 추가
        
        int total = 0;
        printf("결과: [ ");
        for (int i = 0; i < 5; i++) {
            printf("%d ", dice[i]);
            total += dice[i];
        }
        printf("] 합계: %d\n", total);
        
        int threshold = 18; // 성공 기준치
        if (total >= threshold) {
            printf("[성공!] 상자가 가볍게 열렸습니다!\n");
            int gold_reward = 100 + (total - threshold) * 10;
            printf("골드 %d G를 획득했습니다!\n", gold_reward);
            p->gold += gold_reward;
            
            // 낮은 확률로 무기 업그레이드 또는 추가 보너스
            if (total >= 25) {
                printf("상자 안에서 신비한 기운이 느껴집니다. 힘(STR)이 영구적으로 상승합니다! (STR +2)\n");
                p->str += 2;
            }
        } else {
            printf("[실패!] 상자가 닫히며 독침이 발사되었습니다!\n");
            int damage = (int)(p->max_hp *0.2f);
            printf("HP가 %d 만큼 감소했습니다. (최대 체력의 20%%)\n", damage);
            p->hp -= damage;
            if (p->hp < 1) p->hp = 1; // 죽지는 않게 설정
        }
    } else {
        printf("상자를 뒤로하고 길을 떠납니다.\n");
    }
}

static void event_oasis(Player* p) {
    printf("\n[이벤트] 사막의 오아시스를 발견했습니다!\n");
    int heal = (int)(p->max_hp *0.3f);
    printf("맑은 물을 마셔 에너지가 회복됩니다. (HP +%d)\n", heal);
    p->hp += heal;
    if (p->hp > p->max_hp) p->hp = p->max_hp;
}

static void event_trap(Player* p) {
    printf("\n[이벤트] 발밑의 함정을 밟았습니다!\n");
    int damage = (int)(p->max_hp *0.1f);
    printf("날카로운 가시에 찔려 상처를 입었습니다. (HP -%d)\n", damage);
    p->hp -= damage;
    if (p->hp < 1) p->hp = 1;
}

static void event_training_ground(Player* p) {
    printf("\n[활동] 마을 근처의 훈련장을 발견했습니다.\n");
    printf("어떤 훈련을 하시겠습니까?\n");
    printf("1. 목검 휘두르기 (STR +1)\n2. 과녁 맞추기 (DEX +1)\n");
    printf("선택: ");
    int choice;
    if (scanf("%d", &choice) != 1) { clear_input_buffer(); return; }
    clear_input_buffer();
    if (choice == 1) { printf("땀을 흘리며 근력을 키웠습니다! (STR +1)\n"); p->str++; }
    else { printf("집중력을 발휘하여 명중률을 높였습니다! (DEX +1)\n"); p->dex++; }
}

static void event_library(Player* p) {
    printf("\n[활동] 오래된 도서관에서 고서를 읽습니다.\n");
    printf("심오한 지식을 습득하여 머리가 맑아집니다. (INT +1, EXP +50)\n");
    p->intel++;
    p->exp += 50;
    check_level_up(p);
}

static void event_alley_gamble(Player* p) {
    printf("\n[이벤트] 뒷골목에서 수상한 도박판을 발견했습니다.\n");
    printf("한 판 벌여보시겠습니까? (50%% 확률로 LUK +2 또는 Gold -100)\n");
    printf("1. 한다\n2. 지나친다\n");
    printf("선택: ");
    int choice;
    if (scanf("%d", &choice) != 1) { clear_input_buffer(); return; }
    clear_input_buffer();
    if (choice == 1) {
        if (rand() % 2 == 0) {
            printf("심리전에서 승리했습니다! 운이 상승합니다. (LUK +2)\n");
            p->luk += 2;
        } else {
            printf("보기 좋게 탈탈 털렸습니다... (Gold -100)\n");
            p->gold -= 100;
            if (p->gold < 0) p->gold = 0;
        }
    }
}

static void event_mysterious_potion(Player* p) {
    printf("\n[이벤트] 길에 떨어진 정체불명의 약병을 발견했습니다.\n");
    printf("마셔보시겠습니까? (무작위 능력치 변동)\n");
    printf("1. 마신다\n2. 버린다\n");
    printf("선택: ");
    int choice;
    if (scanf("%d", &choice) != 1) { clear_input_buffer(); return; }
    clear_input_buffer();
    if (choice == 1) {
        int r = rand() % 4;
        int amt = (rand() % 3) + 1;
        const char* s_names[] = {"STR", "DEX", "INT", "LUK"};
        printf("몸에서 이상한 반응이 옵니다! (%s +%d)\n", s_names[r], amt);
        if(r==0) p->str += amt;
        else if(r==1) p->dex += amt;
        else if(r==2) p->intel += amt;
        else p->luk += amt;
    }
}

void trigger_event(Player* p) {
    clear_screen();
    show_compact_status(p);
    
    int dice = rand() % 100;

    if (dice < 5) { // 5% Rare
        int rare_dice = rand() % 2;
        if (rare_dice == 0) event_ancient_dragon_altar(p);
        else event_void_sage(p);
    } else if (dice < 20) { // 15% Uncommon (5~19)
        int uncommon_dice = rand() % 2;
        if (uncommon_dice == 0) event_cursed_chest(p);
        else event_mysterious_labyrinth(p);
    } else { // 80% Common
        int event_chance = rand() % 100;

        if (event_chance < 15) {
            event_mysterious_chest(p);
        } else if (event_chance < 25) {
            event_oasis(p);
        } else if (event_chance < 35) {
            event_trap(p);
        } else if (event_chance < 50) {
            event_training_ground(p);
        } else if (event_chance < 65) {
            event_library(p);
        } else if (event_chance < 80) {
            event_alley_gamble(p);
        } else if (event_chance < 90) {
            event_mysterious_potion(p);
        } else {
            // 나머지 일반 텍스트 이벤트
            printf("\n--- 길거리 발견 이벤트 ---\n");
            int sub_event = rand() % 3;
            
            switch (sub_event) {
                case 0:
                    printf("길가에서 누군가 잃어버린 주머니를 발견했습니다!\n");
                    printf("Gold +30 획득!\n");
                    p->gold += 30;
                    break;
                case 1:
                    printf("여행자의 흔적을 따라가다 사과 나무를 발견했습니다.\n");
                    printf("HP +15 회복\n");
                    p->hp += 15;
                    if (p->hp > p->max_hp) p->hp = p->max_hp;
                    break;
                case 2:
                    printf("오래된 비석을 조사하다가 영감을 얻었습니다.\n");
                    printf("EXP +20 획득!\n");
                    p->exp += 20;
                    check_level_up(p);
                    break;
            }
        }
    }
    
    wait_for_enter();
}
