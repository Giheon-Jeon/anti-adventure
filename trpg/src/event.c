#include <stdio.h>
#include <stdlib.h>
#include "../include/event.h"
#include "../include/combat.h"
#include "../include/utils.h"

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
    
    wait_for_enter();
}
