#include "../include/shop.h"
#include "../include/utils.h"
#include <stdio.h>

void open_shop(Player* p) {
    int choice;
    while (1) {
        clear_screen();
        show_compact_status(p);
        print_divider(80, YELLOW);
        print_centered(YELLOW_BG BLACK " [ 🛒 마을 상점 - 리부트 ] " RESET, 80);
        print_divider(80, YELLOW);
        
        char buf[256];
        // 1. 무기 슬롯
        if (p->weapon_tier == 0) sprintf(buf, "1. [무기] 훈련용 목검 (STR+10, DEX+5) - 100 G");
        else if (p->weapon_tier == 1) sprintf(buf, "1. [무기] 강철 세이버 (STR+40, DEX+20) - 500 G");
        else if (p->weapon_tier == 2) sprintf(buf, "1. [무기] 앱솔랩스 소드 (STR+120, DEX+60) - 2500 G");
        else if (p->weapon_tier == 3) sprintf(buf, "1. [무기] 아케인셰이드 (STR+300, DEX+150, 마력+20) - 10000 G");
        else if (p->weapon_tier == 4) sprintf(buf, "1. [무기] 제네시스 소드 (STR+800, DEX+400, 마력+50, 보뎀+10%%) - 50000 G");
        else sprintf(buf, "1. [무기] 품절");
        print_box_line(buf, 80, YELLOW);

        // 2. 방어구 슬롯
        if (p->armor_tier == 0) sprintf(buf, "2. [방어구] 수습 도복 (HP+200, MP+100) - 150 G");
        else if (p->armor_tier == 1) sprintf(buf, "2. [방어구] 네크로 아머 (HP+1000, MP+400) - 800 G");
        else if (p->armor_tier == 2) sprintf(buf, "2. [방어구] 카루타 세트 (HP+4000, MP+2000) - 3000 G");
        else if (p->armor_tier == 3) sprintf(buf, "2. [방어구] 앱솔랩스 세트 (HP+12000, MP+6000) - 15000 G");
        else if (p->armor_tier == 4) sprintf(buf, "2. [방어구] 에테르넬 세트 (HP+40000, MP+20000, 뎀퍼+5%%) - 60000 G");
        else sprintf(buf, "2. [방어구] 품절");
        print_box_line(buf, 80, YELLOW);

        // 3. 장신구 슬롯
        if (p->accessory_tier == 0) sprintf(buf, "3. [장신구] 실버블라썸 링 (AllStat+5) - 300 G");
        else if (p->accessory_tier == 1) sprintf(buf, "3. [장신구] 보스 장신구 세트 (AllStat+20) - 1200 G");
        else if (p->accessory_tier == 2) sprintf(buf, "3. [장신구] 마이스터링 (AllStat+100) - 5000 G");
        else if (p->accessory_tier == 3) sprintf(buf, "3. [장신구] 칠흑의 보스 세트 (AllStat+300, 방무+5%%) - 20000 G");
        else if (p->accessory_tier == 4) sprintf(buf, "3. [장신구] 여명의 보스 세트 (AllStat+800, 방무+10%%) - 80000 G");
        else sprintf(buf, "3. [장신구] 품절");
        print_box_line(buf, 80, YELLOW);

        print_box_line("4. [소비] 빨간 포션 (HP 50%% 회복) - 30 G", 80, YELLOW);
        print_box_line("5. [소비] 파란 포션 (MP 50%% 회복) - 30 G", 80, YELLOW);
        print_box_line("0. 나가기", 80, YELLOW);
        print_divider(80, YELLOW);

        printf("\n 선택: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 0) break;

        switch (choice) {
            case 1:
                if (p->weapon_tier == 0 && p->gold >= 100) {
                    p->gold -= 100; p->str += 10; p->dex += 5; p->weapon_tier++;
                    printf("[구매] 훈련용 목검을 구매했습니다!\n");
                } else if (p->weapon_tier == 1 && p->gold >= 500) {
                    p->gold -= 500; p->str += 40; p->dex += 20; p->weapon_tier++;
                    printf("[구매] 강철 세이버를 구매했습니다!\n");
                } else if (p->weapon_tier == 2 && p->gold >= 2500) {
                    p->gold -= 2500; p->str += 120; p->dex += 60; p->weapon_tier++;
                    printf("[구매] 앱솔랩스 소드를 구매했습니다!\n");
                } else if (p->weapon_tier == 3 && p->gold >= 10000) {
                    p->gold -= 10000; p->str += 300; p->dex += 150; p->magic_atk += 20; p->weapon_tier++;
                    printf("[구매] 아케인셰이드 소드를 구매했습니다!\n");
                } else if (p->weapon_tier == 4 && p->gold >= 50000) {
                    p->gold -= 50000; p->str += 800; p->dex += 400; p->magic_atk += 50; p->boss_dmg += 0.1f; p->weapon_tier++;
                    printf("[구매] 제네시스 소드를 구매했습니다!\n");
                } else if (p->weapon_tier >= 5) {
                    printf("[알림] 더 이상 업그레이드할 수 없습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 2:
                if (p->armor_tier == 0 && p->gold >= 150) {
                    p->gold -= 150; p->max_hp += 200; p->max_mp += 100; p->armor_tier++;
                    printf("[구매] 수습 도복을 구매했습니다!\n");
                } else if (p->armor_tier == 1 && p->gold >= 800) {
                    p->gold -= 800; p->max_hp += 1000; p->max_mp += 400; p->armor_tier++;
                    printf("[구매] 네크로 아머를 구매했습니다!\n");
                } else if (p->armor_tier == 2 && p->gold >= 3000) {
                    p->gold -= 3000; p->max_hp += 4000; p->max_mp += 2000; p->armor_tier++;
                    printf("[구매] 카루타 상하의를 구매했습니다!\n");
                } else if (p->armor_tier == 3 && p->gold >= 15000) {
                    p->gold -= 15000; p->max_hp += 12000; p->max_mp += 6000; p->armor_tier++;
                    printf("[구매] 앱솔랩스 숄더/신발을 구매했습니다!\n");
                } else if (p->armor_tier == 4 && p->gold >= 60000) {
                    p->gold -= 60000; p->max_hp += 40000; p->max_mp += 20000; p->dmg_percent += 0.05f; p->armor_tier++;
                    printf("[구매] 에테르넬 세트를 구매했습니다!\n");
                } else if (p->armor_tier >= 5) {
                    printf("[알림] 더 이상 업그레이드할 수 없습니다.\n");
                } else {
                    printf("[알림] 골드가 부족합니다!\n");
                }
                break;
            case 3:
                if (p->accessory_tier == 0 && p->gold >= 300) {
                    p->gold -= 300; p->str += 5; p->dex += 5; p->intel += 5; p->luk += 5; p->accessory_tier++;
                    printf("[구매] 실버블라썸 링을 구매했습니다!\n");
                } else if (p->accessory_tier == 1 && p->gold >= 1200) {
                    p->gold -= 1200; p->str += 20; p->dex += 20; p->intel += 20; p->luk += 20; p->accessory_tier++;
                    printf("[구매] 보스 장신구 세트를 구매했습니다!\n");
                } else if (p->accessory_tier == 2 && p->gold >= 5000) {
                    p->gold -= 5000; p->str += 100; p->dex += 100; p->intel += 100; p->luk += 100; p->accessory_tier++;
                    printf("[구매] 마이스터링을 구매했습니다!\n");
                } else if (p->accessory_tier == 3 && p->gold >= 20000) {
                    p->gold -= 20000; p->str += 300; p->dex += 300; p->intel += 300; p->luk += 300; p->ied += 0.05f; p->accessory_tier++;
                    printf("[구매] 칠흑의 보스 세트를 구매했습니다!\n");
                } else if (p->accessory_tier == 4 && p->gold >= 80000) {
                    p->gold -= 80000; p->str += 800; p->dex += 800; p->intel += 800; p->luk += 800; p->ied += 0.10f; p->accessory_tier++;
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
