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
    
    if (choice == 1) {
        printf("\n주사위 5개를 던집니다...\n");
        int total = 0;
        printf("결과: [ ");
        for (int i = 0; i < 5; i++) {
            int roll = (rand() % 6) + 1;
            printf("%d ", roll);
            total += roll;
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

void trigger_event(Player* p) {
    clear_screen();
    show_compact_status(p);
    
    int event_chance = rand() % 100;

    if (event_chance < 50) {
        // 50% 확률로 전투 발생
        start_combat(p);
    } else if (event_chance < 70) {
        // 20% 확률로 신비한 상자
        event_mysterious_chest(p);
    } else if (event_chance < 80) {
        // 10% 확률로 오아시스
        event_oasis(p);
    } else if (event_chance < 90) {
        // 10% 확률로 함정
        event_trap(p);
    } else {
        // 10% 확률로 기존 일반 텍스트 이벤트 발생
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
    
    printf("\n엔터를 누르면 계속합니다...");
    while (getchar() != '\n'); getchar();
}
