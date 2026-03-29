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
        else if (p->weapon_tier == 3) printf("1. [무기] 아케인셰이드 소드 (STR +150, DEX +80, 보뎀 +30%%, 뎀퍼 +10%%) - 10000 G\n");
        else if (p->weapon_tier == 4) printf("1. [무기] 제네시스 소드 (STR +400, DEX +200, 보뎀 +50%%, 뎀퍼 +20%%, 방무 +10%%) - 50000 G\n");
        else printf("1. [무기] 품절\n");

        // 2. 방어구 슬롯
        if (p->armor_tier == 0) printf("2. [방어구] 수습 도복 (HP +100, MP +50) - 150 G\n");
        else if (p->armor_tier == 1) printf("2. [방어구] 네크로 아머 (HP +500, MP +200) - 800 G\n");
        else if (p->armor_tier == 2) printf("2. [방어구] 카루타 상하의 (HP +2000, MP +1000, 뎀퍼 +5%%) - 3000 G\n");
        else if (p->armor_tier == 3) printf("2. [방어구] 앱솔랩스 숄더/신발 (HP +6000, MP +3000, 뎀퍼 +15%%) - 15000 G\n");
        else if (p->armor_tier == 4) printf("2. [방어구] 에테르넬 세트 (HP +20000, MP +10000, 뎀퍼 +30%%, 보뎀 +10%%) - 60000 G\n");
        else printf("2. [방어구] 품절\n");

        // 3. 장신구 슬롯
        if (p->accessory_tier == 0) printf("3. [장신구] 실버블라썸 링 (DEX +5, 방무 +5%%) - 300 G\n");
        else if (p->accessory_tier == 1) printf("3. [장신구] 보스 장신구 세트 (DEX +20, 방무 +15%%) - 1200 G\n");
        else if (p->accessory_tier == 2) printf("3. [장신구] 마이스터링 (AllStat +50, 방무 +30%%) - 5000 G\n");
        else if (p->accessory_tier == 3) printf("3. [장신구] 칠흑의 보스 세트 (AllStat +150, 방무 +50%%) - 20000 G\n");
        else if (p->accessory_tier == 4) printf("3. [장신구] 여명의 보스 세트 (AllStat +400, 방무 +70%%, 뎀퍼 +10%%) - 80000 G\n");
        else printf("3. [장신구] 품절\n");

        // 4, 5. 소비 아이템
        printf("4. [포션] 빨간 포션 (HP 50%% 회복) - 30 G\n");
        printf("5. [포션] 파란 포션 (MP 50%% 회복) - 30 G\n");

        printf("0. 나가기\n");
        printf("선택: ");
        scanf("%d", &choice);
        clear_input_buffer(); // 여기서 버퍼를 비워줌

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
                } else if (p->weapon_tier == 3 && p->gold >= 10000) {
                    p->gold -= 10000; p->str += 150; p->dex += 80; p->boss_dmg += 0.3f; p->dmg_percent += 0.1f; p->weapon_tier++;
                    printf("[구매] 아케인셰이드 소드를 구매했습니다!\n");
                } else if (p->weapon_tier == 4 && p->gold >= 50000) {
                    p->gold -= 50000; p->str += 400; p->dex += 200; p->boss_dmg += 0.5f; p->dmg_percent += 0.2f; p->ied += 0.1f; p->weapon_tier++;
                    printf("[구매] 제네시스 소드를 구매했습니다!\n");
                } else if (p->weapon_tier >= 5) {
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
                } else if (p->armor_tier == 3 && p->gold >= 15000) {
                    p->gold -= 15000; p->max_hp += 6000; p->max_mp += 3000; p->dmg_percent += 0.15f; p->armor_tier++;
                    printf("[구매] 앱솔랩스 숄더/신발을 구매했습니다!\n");
                } else if (p->armor_tier == 4 && p->gold >= 60000) {
                    p->gold -= 60000; p->max_hp += 20000; p->max_mp += 10000; p->dmg_percent += 0.30f; p->boss_dmg += 0.1f; p->armor_tier++;
                    printf("[구매] 에테르넬 세트를 구매했습니다!\n");
                } else if (p->armor_tier >= 5) {
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
                } else if (p->accessory_tier == 3 && p->gold >= 20000) {
                    p->gold -= 20000; p->str += 150; p->dex += 150; p->intel += 150; p->luk += 150; p->ied += 0.20f; p->accessory_tier++;
                    printf("[구매] 칠흑의 보스 세트를 구매했습니다!\n");
                } else if (p->accessory_tier == 4 && p->gold >= 80000) {
                    p->gold -= 80000; p->str += 400; p->dex += 400; p->intel += 400; p->luk += 400; p->ied += 0.20f; p->dmg_percent += 0.1f; p->accessory_tier++;
                    printf("[구매] 여명의 보스 세트를 구매했습니다!\n");
                } else if (p->accessory_tier >= 5) {
                    printf("[알림] 더 이상 업그레이드할 수 없습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 4:
                if (p->gold >= 30) {
                    p->gold -= 30;
                    p->hp += p->max_hp / 2;
                    if (p->hp > p->max_hp) p->hp = p->max_hp;
                    printf("[구매] 빨간 포션을 사용했습니다. HP가 회복되었습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 5:
                if (p->gold >= 30) {
                    p->gold -= 30;
                    p->mp += p->max_mp / 2;
                    if (p->mp > p->max_mp) p->mp = p->max_mp;
                    printf("[구매] 파란 포션을 사용했습니다. MP가 회복되었습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            default:
                printf("[알림] 잘못된 선택이거나 품절된 상품입니다.\n");
                break;
        }
        
        if (choice != 0) {
            wait_for_enter();
        }
    }
    printf("상점을 나갑니다.\n");
}
