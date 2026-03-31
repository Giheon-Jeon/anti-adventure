#include "../include/village.h"
#include "../include/crafting.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

void open_village_menu(Player* p) {
    int choice;
    while (1) {
        clear_screen();
        show_compact_status(p);
        printf("\n========= [마을 활동 센터] =========\n");
        printf("전투 외에 안전하게 활동할 수 있습니다.\n");
        printf("1. 파트타임 알바 (마을 광장 청소) - [보상: 30G, 10 HP 회복]\n");
        printf("2. 주사위 도박 (하이/로우) - [최소 배팅: 50G]\n");
        printf("3. 장비 제작 (대장간) - [몬스터 전리품 재재료]\n");
        printf("0. 뒤로 가기\n");
        printf("선택: ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("\n[알바 중...] 마을 광장을 깨끗하게 청소했습니다! ✨\n");
                p->gold += 30;
                p->hp += 10;
                if (p->hp > p->max_hp) p->hp = p->max_hp;
                printf("보상으로 30G를 벌고 몸이 가벼워졌습니다. (+10 HP)\n");
                break;
            case 2:
                if (p->gold < 50) {
                    printf("\n[알림] 배팅할 돈이 부족합니다. (최소 50G 필요)\n");
                } else {
                    int bet;
                    printf("\n--- 주사위 도박 (하이/로우) ---\n");
                    printf("현재 골드: %d G\n", p->gold);
                    printf("배팅할 금액을 입력하세요: ");
                    scanf("%d", &bet);
                    clear_input_buffer();

                    if (bet < 50 || bet > p->gold) {
                        printf("[알림] 잘못된 배팅 금액입니다.\n");
                    } else {
                        printf("눈금을 예측하세요 (1. 1~3:로우 / 2. 4~6:하이): ");
                        int predict;
                        scanf("%d", &predict);
                        clear_input_buffer();

                        int dice = (rand() % 6) + 1;
                        printf("\n주사위를 던집니다... 결과: [ %d ]\n", dice);

                        int is_high = (dice >= 4);
                        if ((predict == 1 && !is_high) || (predict == 2 && is_high)) {
                            printf("✨ [축하합니다!] 예측이 적중했습니다! ✨\n");
                            p->gold += bet;
                            printf("+ %d G 획득! (현재: %d G)\n", bet, p->gold);
                        } else {
                            printf("💀 [실패] 돈을 잃었습니다... 💀\n");
                            p->gold -= bet;
                            printf("- %d G 소실 (현재: %d G)\n", bet, p->gold);
                        }
                    }
                }
                break;
            case 3:
                open_crafting_menu(p);
                break;
            default:
                printf("\n[알림] 잘못된 선택입니다.\n");
                break;
        }
        wait_for_enter();
    }
}
