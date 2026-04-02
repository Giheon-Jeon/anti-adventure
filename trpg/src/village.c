#include "../include/village.h"
#include "../include/crafting.h"
#include "../include/ability.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void open_village_menu(Player* p) {
    int choice;
    while (1) {
        clear_screen();
        show_compact_status(p);
        printf("\n========= [마을 활동 센터] =========\n");
        printf("전투 외에 안전하게 활동할 수 있습니다.\n");
        printf("1. 파트타임 알바 (무작위 보상) - [5회 기동 시 1분 휴식]\n");
        printf("2. 주사위 도박 (하이/로우) - [최소 배팅: 50G]\n");
        printf("3. 장비 제작 (대장간) - [몬스터 전리품 재료]\n");
        printf("4. 어빌리티 잠재력 (신비한 재단) - [골드 소모]\n");
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
                {
                    time_t now = time(NULL);
                    double elapsed = difftime(now, p->last_job_time);
                    
                    if (p->job_count >= 5 && elapsed < 60) {
                        printf("\n[휴식 필요] 너무 무리하게 일했습니다! (남은 시간: %.0f초)\n", 60 - elapsed);
                    } else {
                        if (elapsed >= 60) p->job_count = 0; // 1분 넘게 지났으면 초기화
                        
                        printf("\n[알바 중...] 마을 광장을 열심히 청소합니다... ✨\n");
                        int roll = rand() % 100;
                        int gold = 0, hp = 0;
                        
                        if (roll < 10) { // 대성공
                            gold = 20; hp = 10;
                            printf("✨ [대성공!] 당신의 꼼꼼한 청소 실력에 마을 사람들이 감동했습니다! ✨\n");
                        } else if (roll < 60) { // 성공
                            gold = 15; hp = 5;
                            printf("✅ [성공] 깨끗해진 광장을 보며 보람을 느낍니다.\n");
                        } else if (roll < 90) { // 실패
                            gold = 5; hp = 0;
                            printf("❓ [실패] 청소가 덜 된 부분이 있는 것 같습니다.\n");
                        } else { // 대실패
                            gold = 0; hp = -5;
                            printf("💀 [대실패] 청소 도중 빗자루에 발이 꼬여 넘어졌습니다! 💀\n");
                        }
                        
                        p->gold += gold;
                        p->hp += hp;
                        if (p->hp > p->max_hp) p->hp = p->max_hp;
                        if (p->hp < 1) p->hp = 1;

                        printf("보상: [ %d G / HP %+d ]\n", gold, hp);
                        
                        p->job_count++;
                        p->last_job_time = time(NULL);
                        
                        if (p->job_count >= 5) {
                            printf("🚨 너무 많이 일했습니다! 이제 좀 쉬어야 합니다. (1분 쿨타임 발생) 🚨\n");
                        }
                    }
                }
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
            case 4:
                open_ability_menu(p);
                break;
            default:
                printf("\n[알림] 잘못된 선택입니다.\n");
                break;
        }
        wait_for_enter();
    }
}
