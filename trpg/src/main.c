#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/character.h"
#include "../include/event.h"
#include "../include/shop.h"
#include "../include/utils.h"
#include "../include/saveload.h"

#include "../include/village.h"
#include "../include/combat.h"

int main() {
    // Windows 터미널 한글 깨짐 방지
    system("chcp 65001 > nul");

    srand((unsigned int)time(NULL));
    
    // 타이틀 화면 (새로 시작 / 이어하기 선택)
    int title_choice = show_title_screen();
    
    Player player;
    
    if (title_choice == 2) {
        // 이어하기: 세이브 파일 목록에서 선택
        if (!show_load_menu(&player)) {
            // 불러오기 실패/취소 → 새로 시작으로 전환
            printf("\n" YELLOW "세이브 파일을 불러올 수 없습니다. 새 게임을 시작합니다." RESET "\n");
            wait_for_enter();
            init_player(&player);
        }
    } else {
        // 새로 시작
        init_player(&player);
    }
    
    int choice;
    while (1) {
        clear_screen();
        show_compact_status(&player);
        
        printf("\n " CYAN " [ 메인 메뉴 ] " RESET "\n");
        print_divider(80, CYAN);
        
        printf("  1. " GREEN "사냥하기 " RESET "(던전 선택)\n");
        printf("  2. " YELLOW "모험하기 " RESET "(무작위 이벤트)\n");
        printf("  3. " WHITE "상점 방문 " RESET "\n");
        printf("  4. " CYAN "마을 활동 " RESET "(알바/도박)\n");
        printf("  5. " BOLD "인벤토리 보기" RESET "\n");
        printf("  6. " BOLD "캐릭터 상세정보" RESET "\n");
        printf("  7. " MAGENTA BOLD "스킬 트리 " RESET "\n");
        printf("  8. " YELLOW BOLD "💾 저장하기" RESET "\n");
        printf("  9. " RED "게임 종료" RESET "\n");
        
        print_divider(80, CYAN);
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
                show_skill_tree(&player);
                break;
            case 8:
                show_save_menu(&player);
                break;
            case 9: {
                // 종료 전 저장 확인
                printf("\n" YELLOW BOLD "  게임을 종료하기 전에 저장하시겠습니까? (Y/N): " RESET);
                char save_confirm;
                scanf(" %c", &save_confirm);
                clear_input_buffer();
                if (save_confirm == 'Y' || save_confirm == 'y') {
                    show_save_menu(&player);
                }
                printf("\n" YELLOW BOLD "  게임을 종료합니다. 안녕히 가세요!" RESET "\n");
                return 0;
            }
            default:
                printf("\n" RED "잘못된 선택입니다. 다시 입력해주세요." RESET "\n");
                wait_for_enter();
        }
    }
    
    return 0;
}
