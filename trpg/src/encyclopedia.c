#include "../include/character.h"
#include "../include/utils.h"
#include "../include/generated_data.h"
#include <stdio.h>
#include <string.h>

void show_monster_encyclopedia(Player* p) {
    while (1) {
        clear_screen();
        print_divider(80, MAGENTA);
        print_centered(MAGENTA_BG BLACK " [ 📜 몬스터 도감 - 지식의 기록 ] " RESET, 80);
        print_divider(80, MAGENTA);
        print_box_line("등록된 몬스터의 정보를 확인하고 연구 등급을 높이세요.", 80, MAGENTA);
        print_divider(80, MAGENTA);

        int registered_count = 0;
        for (int i = 0; i < MAX_BOOK_ENTRIES; i++) {
            if (p->encyclopedia[i].id > 0 && p->encyclopedia[i].is_registered) {
                registered_count++;
            }
        }
        
        char buf[256];
        sprintf(buf, "전체 등록 현황: [ %d / %d ]", registered_count, MONSTER_DB_SIZE);
        print_box_line(buf, 80, MAGENTA);
        print_divider(80, MAGENTA);

        for (int i = 0; i < MONSTER_DB_SIZE; i++) {
            int mid = g_monster_db[i].id;
            MonsterBookEntry* entry = NULL;
            
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

                sprintf(buf, "%d. %s %s | 연구 Lv.%d (%d 처치)", 
                        mid, g_monster_db[i].name, 
                        g_monster_db[i].is_boss ? "[BOSS]" : "",
                        level, entry->kill_count);
                print_box_line(buf, 80, YELLOW);
                
                if (level >= 1) {
                    sprintf(buf, "   ↳ HP: %d | 방어율: %d", g_monster_db[i].max_hp, g_monster_db[i].def);
                    print_box_line(buf, 80, WHITE);
                }
                if (level >= 2) {
                    print_box_line("   ↳ [효과] 해당 몬스터 상대 데미지 +20% 증가", 80, GREEN);
                }
                if (level >= 3) {
                    if (g_monster_db[i].is_boss) {
                        print_box_line("   ↳ [효과] 보스 정밀 분석 완료 (약점 노출)", 80, CYAN);
                    } else {
                        print_box_line("   ↳ [효과] 자동 사냥 가능 (지식 숙달)", 80, CYAN);
                    }
                }
            } else {
                sprintf(buf, "%d. [ ????? ] (아직 발견하지 못함)", mid);
                print_box_line(buf, 80, WHITE);
            }
            if (i < MONSTER_DB_SIZE - 1) print_divider(80, MAGENTA);
        }

        print_divider(80, MAGENTA);
        print_box_line("0. 뒤로 가기", 80, MAGENTA);
        print_divider(80, MAGENTA);
        printf("\n 선택: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 0) break;
    }
}
