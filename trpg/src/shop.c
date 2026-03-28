#include "../include/shop.h"
#include "../include/utils.h"
#include <stdio.h>

void open_shop(Player* p) {
    int choice;
    while (1) {
        clear_screen();
        show_compact_status(p);
        printf("\n========= [마을 상점 - 리부트] =========\n");
        
        // 1. 무기 슬롯
        if (p->weapon_tier == 0) printf("1. [무기] 훈련용 목검 (STR +5, DEX +2) - 100 G\n");
        else if (p->weapon_tier == 1) printf("1. [무기] 강철 세이버 (STR +20, DEX +10) - 500 G\n");
        else if (p->weapon_tier == 2) printf("1. [무기] 앱솔랩스 소드 (STR +60, DEX +30, 보뎀 +20%%) - 2500 G\n");
        else printf("1. [무기] 품절\n");

        // 2. 방어구 슬롯
        if (p->armor_tier == 0) printf("2. [방어구] 수습 도복 (HP +100, MP +50) - 150 G\n");
        else if (p->armor_tier == 1) printf("2. [방어구] 네크로 아머 (HP +500, MP +200) - 800 G\n");
        else if (p->armor_tier == 2) printf("2. [방어구] 카루타 상하의 (HP +2000, MP +1000, 뎀퍼 +5%%) - 3000 G\n");
        else printf("2. [방어구] 품절\n");

        // 3. 장신구 슬롯
        if (p->accessory_tier == 0) printf("3. [장신구] 실버블라썸 링 (DEX +5, 방무 +5%%) - 300 G\n");
        else if (p->accessory_tier == 1) printf("3. [장신구] 보스 장신구 세트 (DEX +20, 방무 +15%%) - 1200 G\n");
        else if (p->accessory_tier == 2) printf("3. [장신구] 마이스터링 (AllStat +50, 방무 +30%%) - 5000 G\n");
        else printf("3. [장신구] 품절\n");

        printf("0. 나가기\n");
        printf("선택: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
            case 1:
                if (p->weapon_tier == 0 && p->gold >= 100) {
                    p->gold -= 100; p->str += 5; p->dex += 2; p->weapon_tier++;
                    printf("[구매] 훈련용 목검을 구매했습니다!\n");
                } else if (p->weapon_tier == 1 && p->gold >= 500) {
                    p->gold -= 500; p->str += 20; p->dex += 10; p->weapon_tier++;
                    printf("[구매] 강철 세이버를 구매했습니다!\n");
                } else if (p->weapon_tier == 2 && p->gold >= 2500) {
                    p->gold -= 2500; p->str += 60; p->dex += 30; p->boss_dmg += 0.2f; p->weapon_tier++;
                    printf("[구매] 앱솔랩스 소드를 구매했습니다!\n");
                } else if (p->weapon_tier >= 3) {
                    printf("[알림] 더 이상 업그레이드할 수 없습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 2:
                if (p->armor_tier == 0 && p->gold >= 150) {
                    p->gold -= 150; p->max_hp += 100; p->max_mp += 50; p->armor_tier++;
                    printf("[구매] 수습 도복을 구매했습니다!\n");
                } else if (p->armor_tier == 1 && p->gold >= 800) {
                    p->gold -= 800; p->max_hp += 500; p->max_mp += 200; p->armor_tier++;
                    printf("[구매] 네크로 아머를 구매했습니다!\n");
                } else if (p->armor_tier == 2 && p->gold >= 3000) {
                    p->gold -= 3000; p->max_hp += 2000; p->max_mp += 1000; p->dmg_percent += 0.05f; p->armor_tier++;
                    printf("[구매] 카루타 상하의를 구매했습니다!\n");
                } else if (p->armor_tier >= 3) {
                    printf("[알림] 더 이상 업그레이드할 수 없습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 3:
                if (p->accessory_tier == 0 && p->gold >= 300) {
                    p->gold -= 300; p->dex += 5; p->ied += 0.05f; p->accessory_tier++;
                    printf("[구매] 실버블라썸 링을 구매했습니다!\n");
                } else if (p->accessory_tier == 1 && p->gold >= 1200) {
                    p->gold -= 1200; p->dex += 20; p->ied += 0.15f; p->accessory_tier++;
                    printf("[구매] 보스 장신구 세트를 구매했습니다!\n");
                } else if (p->accessory_tier == 2 && p->gold >= 5000) {
                    p->gold -= 5000; p->str += 50; p->dex += 50; p->intel += 50; p->luk += 50; p->ied += 0.30f; p->accessory_tier++;
                    printf("[구매] 마이스터링을 구매했습니다!\n");
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
        
        if (choice != 0) {
            printf("\n엔터를 누르면 계속합니다...");
            while (getchar() != '\n'); 
            getchar();
        }
    }
    printf("상점을 나갑니다.\n");
}
