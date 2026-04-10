#include "../include/character.h"
#include "../include/utils.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    int hp;
    int def;
    int is_boss;
} MonsterData;

static MonsterData world_monsters[] = {
    {1, "파란 달팽이", 300, 5, 0},
    {2, "슬라임", 600, 10, 0},
    {3, "주황버섯", 1500, 30, 0},
    {4, "머쉬맘", 10000, 100, 1},
    {5, "오크", 8000, 80, 0},
    {6, "와일드카고", 15000, 150, 0},
    {7, "자쿰의 팔", 50000, 300, 1},
    {8, "스켈레톤", 50000, 300, 0},
    {9, "다크 레인저", 80000, 400, 0},
    {10, "파풀라투스", 300000, 600, 1},
    {11, "블루 와이번", 200000, 600, 0},
    {12, "다크 코르니안", 400000, 800, 0},
    {13, "핑크빈", 1200000, 1200, 1}
};

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
        
        printf(WHITE "전체 등록 현황: [ " GREEN BOLD "%d" RESET " / " WHITE "13" RESET " ]\n", registered_count);
        print_divider(50, MAGENTA);

        for (int i = 0; i < 13; i++) {
            int mid = world_monsters[i].id;
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

                printf(YELLOW BOLD "%2d. %s" RESET, mid, world_monsters[i].name);
                if (world_monsters[i].is_boss) printf(RED " [BOSS]" RESET);
                printf(" | " CYAN "연구 Lv.%d" RESET " (%d 처치)\n", level, entry->kill_count);
                
                if (level >= 1) {
                    printf("   " WHITE "↳ HP: %d | 방어율: %d" RESET "\n", world_monsters[i].hp, world_monsters[i].def);
                }
                if (level >= 2) {
                    printf(GREEN "   ↳ [효과] 데미지 +20%% 증가" RESET "\n");
                }
                if (level >= 3) {
                    if (world_monsters[i].is_boss) {
                        printf(CYAN "   ↳ [효과] 보스 정밀 분석 완료" RESET "\n");
                    } else {
                        printf(CYAN "   ↳ [효과] 자동 사냥 가능" RESET "\n");
                    }
                }
            } else {
                printf(WHITE "%2d. [ ????? ] (미등록)" RESET "\n", mid);
            }
            if (i < 12) printf("------------------------------------\n");
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
