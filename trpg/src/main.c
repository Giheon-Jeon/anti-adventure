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
    
    show_title_screen();
    
    Player player;
    init_player(&player);
    
    int choice;
    while (1) {
        clear_screen();
        show_compact_status(&player);
        
        printf(CYAN BOLD "  [ 메인 메뉴 ]  \n" RESET);
        print_divider(40, CYAN);
        printf("  1. " GREEN "사냥하기 " RESET "(던전 선택)\n");
        printf("  2. " YELLOW "모험하기 " RESET "(무작위 이벤트)\n");
        printf("  3. " WHITE "상점 방문 " RESET "\n");
        printf("  4. " CYAN "마을 활동 " RESET "(알바/도박)\n");
        printf("  5. " BOLD "인벤토리 보기" RESET "\n");
        printf("  6. " BOLD "캐릭터 상세정보" RESET "\n");
        printf("  7. " RED "게임 종료" RESET "\n");
        print_divider(40, CYAN);
        
        printf("\n  선택: ");
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
                printf("\n" YELLOW BOLD "게임을 종료합니다. 안녕히 가세요!" RESET "\n");
                return 0;
            default:
                printf("\n" RED "잘못된 선택입니다. 다시 입력해주세요." RESET "\n");
                wait_for_enter();
        }
    }
    
    return 0;
}
