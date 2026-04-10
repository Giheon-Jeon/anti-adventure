#include "../include/character.h"
#include "../include/utils.h"
#include "../include/generated_data.h"
#include <stdio.h>
#include <string.h>

void show_monster_encyclopedia(Player* p) {
    while (1) {
        clear_screen();
        printf("\n" MAGENTA BOLD "========= [📜 몬스터 도감] =========" RESET "\n");
        printf("등록된 몬스터의 정보를 확인하고 연구 등급을 높이세요.\n\n");

        int registered_count = 0;
        for (int i = 0; i < MAX_BOOK_ENTRIES; i++) {
            if (p->encyclopedia[i].id > 0 && p->encyclopedia[i].is_registered) {
                registered_count++;
            }
        }
        
        printf(WHITE "전체 등록 현황: [ " GREEN BOLD "%d" RESET " / " WHITE "%d" RESET " ]\n", registered_count, MONSTER_DB_SIZE);
        print_divider(50, MAGENTA);

        for (int i = 0; i < MONSTER_DB_SIZE; i++) {
            int mid = g_monster_db[i].id;
            MonsterBookEntry* entry = NULL;
            
            // 플레이어의 도감 데이터에서 해당 ID 찾기
            for (int j = 0; j < MAX_BOOK_ENTRIES; j++) {
                if (p->encyclopedia[j].id == mid) {
                    entry = &p->encyclopedia[j];
                    break;
                }
            }

            if (entry && entry->is_registered) {
                int level = 1;
                if (entry->kill_count >= 30) level = 3;
                else if (entry->kill_count >= 10) level = 2;

                printf(YELLOW BOLD "%2d. %s" RESET, mid, g_monster_db[i].name);
                if (g_monster_db[i].is_boss) printf(RED " [BOSS]" RESET);
                printf(" | " CYAN "연구 Lv.%d" RESET " (%d 처치)\n", level, entry->kill_count);
                
                if (level >= 1) {
                    printf("   " WHITE "↳ HP: %d | 방어율: %d" RESET "\n", g_monster_db[i].max_hp, g_monster_db[i].def);
                }
                if (level >= 2) {
                    printf(GREEN "   ↳ [효과] 데미지 +20%% 증가" RESET "\n");
                }
                if (level >= 3) {
                    if (g_monster_db[i].is_boss) {
                        printf(CYAN "   ↳ [효과] 보스 정밀 분석 완료" RESET "\n");
                    } else {
                        printf(CYAN "   ↳ [효과] 자동 사냥 가능" RESET "\n");
                    }
                }
            } else {
                printf(WHITE "%2d. [ ????? ] (미등록)" RESET "\n", mid);
            }
            if (i < MONSTER_DB_SIZE - 1) printf("------------------------------------\n");
        }

        print_divider(50, MAGENTA);
        printf("0. 뒤로 가기\n");
        printf("선택: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 0) break;
    }
}
