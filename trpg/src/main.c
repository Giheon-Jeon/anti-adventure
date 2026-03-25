#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/character.h"
#include "../include/combat.h"
#include "../include/shop.h"

// TODO: 학생 스스로 combat.c와 shop.c를 구현해보도록 안내
void dummy_combat(Player* p) {
    printf("\n[전투 시스템 준비 중...] 아직 구현되지 않았습니다.\n");
}

void dummy_shop(Player* p) {
    printf("\n[상점 시스템 준비 중...] 아직 구현되지 않았습니다.\n");
}

int main() {
    // Windows 터미널 한글 깨짐 방지: 코드 페이지를 UTF-8(65001)로 변경
    system("chcp 65001 > nul");

    srand((unsigned int)time(NULL));
    
    Player player;
    init_player(&player);
    
    int choice;
    while (1) {
        show_status(&player);
        printf("\n1. 모험하기 (전투)\n2. 상점 방문\n3. 종료\n선택: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                start_combat(&player); // (학생이 채워넣을 부분)
                break;
            case 2:
                open_shop(&player);    // (학생이 채워넣을 부분)
                break;
            case 3:
                printf("게임을 종료합니다. 안녕히 가세요!\n");
                return 0;
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
    
    return 0;
}
