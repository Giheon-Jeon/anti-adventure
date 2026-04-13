#include "../include/village.h"
#include "../include/crafting.h"
#include "../include/ability.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --- 대장간 메뉴 (장비 수리 기능 신규 추가) ---
void open_blacksmith_menu(Player* p) {
    clear_screen();
    print_divider(60, YELLOW);
    print_centered(YELLOW_BG BLACK " [ ⚒️ 대장간 - 수리 서비스 ] " RESET, 60);
    print_divider(60, YELLOW);
    
    char buf[128];
    sprintf(buf, "일반 무기 내구도:   [%3d / 100]", p->weapon_dur);
    print_box_line(buf, 60, YELLOW);
    sprintf(buf, "일반 방어구 내구도: [%3d / 100]", p->armor_dur);
    print_box_line(buf, 60, YELLOW);
    sprintf(buf, "제작 무기 내구도:   [%3d / 100]", p->c_weapon_dur);
    print_box_line(buf, 60, YELLOW);
    sprintf(buf, "제작 방어구 내구도: [%3d / 100]", p->c_armor_dur);
    print_box_line(buf, 60, YELLOW);
    
    print_divider(60, YELLOW);

    int cost = (100 - p->weapon_dur) * 10 + (100 - p->armor_dur) * 10 + (100 - p->accessory_dur) * 10;
    cost += (100 - p->c_weapon_dur) * 20 + (100 - p->c_armor_dur) * 20 + (100 - p->c_accessory_dur) * 20;

    if (cost == 0) {
        printf(GREEN "\n이미 모든 장비가 최상의 상태입니다!\n" RESET);
        wait_for_enter();
        return;
    }

    printf(BOLD "\n총 수리 예상 비용: " YELLOW "%d G" RESET "\n", cost);
    printf("현재 보유 골드: " GREEN "%d G" RESET "\n\n", p->gold);
    
    printf("1. 모든 장비 즉시 수리\n");
    printf("0. 돌아가기\n");
    printf("선택: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) { clear_input_buffer(); return; }
    
    if (choice == 1) {
        if (p->gold >= cost) {
            p->gold -= cost;
            p->weapon_dur = p->armor_dur = p->accessory_dur = 100;
            p->c_weapon_dur = p->c_armor_dur = p->c_accessory_dur = 100;
            printf(GREEN_BG BLACK "\n[수리 완료] 장비들이 다시 날카롭게 빛납니다! ✨" RESET "\n");
        } else {
            printf(RED_BG BLACK "\n[경고] 골드가 부족합니다!" RESET "\n");
        }
        wait_for_enter();
    }
}

void open_village_menu(Player* p) {
    int choice;
    while (1) {
        clear_screen();
        clear_screen();
        show_compact_status(p);
        print_divider(60, CYAN);
        print_centered(BOLD "========= [마을 활동 센터] =========" RESET, 60);
        print_divider(60, CYAN);
        print_box_line("1. 파트타임 알바 (무작위 보상)", 60, CYAN);
        print_box_line("2. 주사위 도박 (하이/로우)", 60, CYAN);
        print_box_line("3. 장비 제작 (대장간)", 60, CYAN);
        print_box_line("4. 어빌리티 잠재력 (신비한 재단)", 60, CYAN);
        print_box_line("5. 몬스터 도감 (지식의 기록)", 60, CYAN);
        print_box_line("6. 장비 수리 (대장간)", 60, CYAN);
        print_box_line("0. 뒤로 가기", 60, CYAN);
        print_divider(60, CYAN);
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
                        if (elapsed >= 60) p->job_count = 0;
                        printf("\n[알바 중...] 마을 광장을 열심히 청소합니다... ✨\n");
                        int roll = rand() % 100;
                        int gold = 0, hp = 0;
                        if (roll < 10) { gold = 20; hp = 10; printf("✨ [대성공!] 사람들에게 칭찬을 들었습니다! ✨\n"); }
                        else if (roll < 60) { gold = 15; hp = 5; printf("✅ [성공] 무난하게 청소를 마쳤습니다.\n"); }
                        else if (roll < 90) { gold = 5; hp = 0; printf("❓ [실패] 청소가 덜 된 곳이 있습니다.\n"); }
                        else { gold = 0; hp = -5; printf("💀 [대실패] 허리를 삐끗했습니다! 💀\n"); }
                        p->gold += gold; p->hp += hp;
                        if (p->hp > p->max_hp) p->hp = p->max_hp;
                        if (p->hp < 1) p->hp = 1;
                        printf("보상: [ %d G / HP %+d ]\n", gold, hp);
                        p->job_count++; p->last_job_time = time(NULL);
                    }
                }
                break;
            case 2:
                if (p->gold < 50) {
                    printf("\n[알림] 배팅할 돈이 부족합니다.\n");
                } else {
                    int bet;
                    printf("\n--- 주사위 도박 (하이/로우) ---\n");
                    printf("배팅할 금액을 입력하세요: ");
                    scanf("%d", &bet); clear_input_buffer();
                    if (bet < 50 || bet > p->gold) { printf("[알림] 잘못된 배팅 금액입니다.\n"); }
                    else {
                        printf("눈금을 예측하세요 (1. 1~3:로우 / 2. 4~6:하이): ");
                        int predict; scanf("%d", &predict); clear_input_buffer();
                        int dice = (rand() % 6) + 1;
                        printf("\n주사위 결과: [ %d ]\n", dice);
                        int is_high = (dice >= 4);
                        if ((predict == 1 && !is_high) || (predict == 2 && is_high)) {
                            printf("✨ [적중!] 예측이 맞았습니다! ✨\n"); p->gold += bet;
                        } else {
                            printf("💀 [실패] 돈을 잃었습니다... 💀\n"); p->gold -= bet;
                        }
                    }
                }
                break;
            case 3: open_crafting_menu(p); break;
            case 4: open_ability_menu(p); break;
            case 5: show_monster_encyclopedia(p); break;
            case 6: open_blacksmith_menu(p); break;
            default: printf("\n[알림] 잘못된 선택입니다.\n"); break;
        }
        wait_for_enter();
    }
}
