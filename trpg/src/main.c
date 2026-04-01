#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/character.h"
#include "../include/event.h"
#include "../include/shop.h"
#include "../include/utils.h"

#include "../include/village.h"
#include "../include/combat.h"

int main() {
    // Windows 터미널 한글 깨짐 방지
    system("chcp 65001 > nul");

    srand((unsigned int)time(NULL));
    
    Player player;
    init_player(&player);
    
    int choice;
    while (1) {
        clear_screen();
        show_compact_status(&player);
        printf("\n1. 사냥하기 (던전 선택)\n2. 모험하기 (무작위 이벤트)\n3. 상점 방문\n4. 마을 활동 (알바/도박)\n5. 인벤토리 보기\n6. 캐릭터 상세정보\n7. 종료\n선택: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1:
                select_dungeon(&player);
                break;
            case 2:
                trigger_event(&player);
                break;
            case 3:
                open_shop(&player);
                break;
            case 4:
                open_village_menu(&player);
                break;
            case 5:
                clear_screen();
                show_inventory(&player);
                break;
            case 6:
                clear_screen();
                show_status(&player);
                wait_for_enter();
                break;
            case 7:
                printf("게임을 종료합니다. 안녕히 가세요!\n");
                return 0;
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
    
    return 0;
}
