#include <stdio.h>
#include "../include/shop.h"

void open_shop(Player* p) {
    int choice;
    while (1) {
        printf("\n========= [마을 상점 - 프리미엄] =========\n");
        printf("현재 보유 골드: %d G\n", p->gold);
        
        // 1. 무기 슬롯
        if (p->weapon_tier == 0) printf("1. [무기] 낡은 검 (+2 데미지) - 50 G\n");
        else if (p->weapon_tier == 1) printf("1. [무기] 철제 장검 (+10 데미지) - 300 G\n");
        else if (p->weapon_tier == 2) printf("1. [무기] 용의 검 (+30 데미지, +0.2x 주사위) - 1000 G\n");
        else printf("1. [무기] 품절\n");

        // 2. 방어구 슬롯
        if (p->armor_tier == 0) printf("2. [방어구] 판금 갑옷 (+5 방어력) - 200 G\n");
        else if (p->armor_tier == 1) printf("2. [방어구] 강화 판금 (+15 방어력) - 500 G\n");
        else if (p->armor_tier == 2) printf("2. [방어구] 드워프 갑옷 (+40 방어력) - 1200 G\n");
        else printf("2. [방어구] 품절\n");

        // 3. 장신구/주사위 슬롯
        if (p->accessory_tier == 0) printf("3. [장신구] 강화된 주사위 (+0.1x 배수) - 150 G\n");
        else if (p->accessory_tier == 1) printf("3. [장신구] 전설의 주사위 (+0.3x 배수) - 600 G\n");
        else if (p->accessory_tier == 2) printf("3. [장신구] 신의 주사위 (+0.6x 배수) - 1500 G\n");
        else printf("3. [장신구] 품절\n");

        printf("0. 나가기\n");
        printf("선택: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
            case 1:
                if (p->weapon_tier == 0 && p->gold >= 50) {
                    p->gold -= 50; p->base_dmg += 2; p->weapon_tier++;
                    printf("[구매] 낡은 검을 구매했습니다!\n");
                } else if (p->weapon_tier == 1 && p->gold >= 300) {
                    p->gold -= 300; p->base_dmg += 10; p->weapon_tier++;
                    printf("[구매] 철제 장검을 구매했습니다!\n");
                } else if (p->weapon_tier == 2 && p->gold >= 1000) {
                    p->gold -= 1000; p->base_dmg += 30; p->dice_multiplier += 0.2f; p->weapon_tier++;
                    printf("[구매] 용의 검을 구매했습니다!\n");
                } else if (p->weapon_tier >= 3) {
                    printf("[알림] 더 이상 업그레이드할 수 없습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 2:
                if (p->armor_tier == 0 && p->gold >= 200) {
                    p->gold -= 200; p->def += 5; p->armor_tier++;
                    printf("[구매] 판금 갑옷을 구매했습니다!\n");
                } else if (p->armor_tier == 1 && p->gold >= 500) {
                    p->gold -= 500; p->def += 15; p->armor_tier++;
                    printf("[구매] 강화 판금을 구매했습니다!\n");
                } else if (p->armor_tier == 2 && p->gold >= 1200) {
                    p->gold -= 1200; p->def += 40; p->armor_tier++;
                    printf("[구매] 드워프 갑옷을 구매했습니다!\n");
                } else if (p->armor_tier >= 3) {
                    printf("[알림] 더 이상 업그레이드할 수 없습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 3:
                if (p->accessory_tier == 0 && p->gold >= 150) {
                    p->gold -= 150; p->dice_multiplier += 0.1f; p->accessory_tier++;
                    printf("[구매] 강화된 주사위를 구매했습니다!\n");
                } else if (p->accessory_tier == 1 && p->gold >= 600) {
                    p->gold -= 600; p->dice_multiplier += 0.3f; p->accessory_tier++;
                    printf("[구매] 전설의 주사위를 구매했습니다!\n");
                } else if (p->accessory_tier == 2 && p->gold >= 1500) {
                    p->gold -= 1500; p->dice_multiplier += 0.6f; p->accessory_tier++;
                    printf("[구매] 신의 주사위를 구매했습니다!\n");
                } else if (p->accessory_tier >= 3) {
                    printf("[알림] 더 이상 업그레이드할 수 없습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            default:
                printf("[알림] 잘못된 선택이거나 품절된 상품입니다.\n");
                break;
        }
    }
    printf("상점을 나갑니다.\n");
}
