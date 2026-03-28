#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/character.h"
#include "../include/event.h"
#include "../include/shop.h"
#include "../include/utils.h"

#include "../include/village.h"

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
        printf("\n1. 모험하기 (전투 및 이벤트)\n2. 상점 방문\n3. 마을 활동 (알바/도박)\n4. 캐릭터 상세정보\n5. 종료\n선택: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1:
                trigger_event(&player); // (전투 또는 이벤트 발생)
                break;
            case 2:
                open_shop(&player);
                break;
            case 3:
                open_village_menu(&player);
                break;
            case 4:
                clear_screen();
                show_status(&player);
                wait_for_enter();
                break;
            case 5:
                printf("게임을 종료합니다. 안녕히 가세요!\n");
                return 0;
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
    
    return 0;
}
