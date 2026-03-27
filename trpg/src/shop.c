#include <stdio.h>
#include "../include/shop.h"

void open_shop(Player* p) {
    int choice;
    while (1) {
        printf("\n========= [마을 상점] =========\n");
        printf("현재 보유 골드: %d G\n", p->gold);
        printf("1. 낡은 검 (공격력 +2) - 50 G\n");
        printf("2. 연습용 목검 (치명타 확률 +5%%) - 80 G\n");
        printf("3. 예리한 단검 (치명타 배율 +0.2x) - 120 G\n");
        printf("4. 강화된 주사위 (주사위 배수 +0.1x) - 150 G\n");
        printf("5. 철제 장검 (공격력 +5, 주사위 배수 +0.1x) - 300 G\n");
        printf("0. 나가기\n");
        printf("선택: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
            case 1:
                if (p->gold >= 50) {
                    p->gold -= 50;
                    p->base_dmg += 2;
                    printf("[구매 완료] 낡은 검을 구매했습니다! (기본 데미지 +2)\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 2:
                if (p->gold >= 80) {
                    p->gold -= 80;
                    p->crit_chance += 0.05f;
                    printf("[구매 완료] 연습용 목검을 구매했습니다! (치명타 확률 +5%%)\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 3:
                if (p->gold >= 120) {
                    p->gold -= 120;
                    p->crit_dmg += 0.2f;
                    printf("[구매 완료] 예리한 단검을 구매했습니다! (치명타 배율 +0.2x)\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 4:
                if (p->gold >= 150) {
                    p->gold -= 150;
                    p->dice_multiplier += 0.1f;
                    printf("[구매 완료] 강화된 주사위를 구매했습니다! (주사위 배수 +0.1x)\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 5:
                if (p->gold >= 300) {
                    p->gold -= 300;
                    p->base_dmg += 5;
                    p->dice_multiplier += 0.1f;
                    printf("[구매 완료] 철제 장검을 구매했습니다! (공격력 +5, 주사위 배수 +0.1x)\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            default:
                printf("[알림] 잘못된 선택입니다.\n");
                break;
        }
    }
    printf("상점을 나갑니다.\n");
}
