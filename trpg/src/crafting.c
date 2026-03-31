#include "../include/crafting.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 재료 보유량 확인 함수
int get_material_count(Player* p, const char* name) {
    int count = 0;
    for (int i = 0; i < p->inventory.count; i++) {
        if (strcmp(p->inventory.items[i].name, name) == 0) {
            count++;
        }
    }
    return count;
}

// 재료 소비 함수 (loss_rate 확률로 소실)
void consume_materials_logic(Player* p, const char* name, int required, int loss_rate, int is_success) {
    int consumed = 0;
    for (int i = 0; i < p->inventory.count && consumed < required; ) {
        if (strcmp(p->inventory.items[i].name, name) == 0) {
            // 성공하면 무조건 소비, 실패하면 loss_rate 확률로 소실
            int should_remove = is_success || (rand() % 100 < loss_rate);
            
            if (should_remove) {
                // 아이템 제거 (배열 앞당기기)
                for (int j = i; j < p->inventory.count - 1; j++) {
                    p->inventory.items[j] = p->inventory.items[j + 1];
                }
                p->inventory.count--;
                if (!is_success) printf("  - [소실] 재료 '%s'(을)를 잃었습니다.\n", name);
            } else {
                if (!is_success) printf("  - [보존] 재료 '%s'(은)는 무사합니다.\n", name);
                i++; // 제거 안 했으므로 다음 인덱스로
            }
            consumed++;
        } else {
            i++;
        }
    }
}

void attempt_craft(Player* p, Recipe recipe) {
    // 1. 재료 확인
    for (int i = 0; i < recipe.material_count; i++) {
        if (get_material_count(p, recipe.materials[i].material_name) < recipe.materials[i].count) {
            printf("\n[오류] 재료가 부족합니다: %s (%d개 필요)\n", 
                   recipe.materials[i].material_name, recipe.materials[i].count);
            return;
        }
    }

    printf("\n🔨 제작 시도 중: Tier %d 장비 (성공 확률 %d%%) 🔨\n", recipe.target_tier, recipe.success_rate);
    wait_for_enter();

    int roll = rand() % 100;
    int is_success = (roll < recipe.success_rate);

    if (is_success) {
        printf("\n✨ [성공] 아이템 제작에 성공했습니다! ✨\n");
        
        // 재료 소비 (성공 시엔 100% 소비)
        for (int i = 0; i < recipe.material_count; i++) {
            consume_materials_logic(p, recipe.materials[i].material_name, recipe.materials[i].count, 100, 1);
        }

        // 스탯 적용 및 티어 상승 (shop.c 로직 참조)
        if (recipe.craft_type == CRAFT_TYPE_WEAPON) {
            if (recipe.target_tier == 1) { p->str += 20; p->dex += 10; }
            else if (recipe.target_tier == 2) { p->str += 60; p->dex += 30; p->boss_dmg += 0.2f; }
            else if (recipe.target_tier == 3) { p->str += 150; p->dex += 80; p->boss_dmg += 0.3f; p->dmg_percent += 0.1f; }
            else if (recipe.target_tier == 4) { p->str += 400; p->dex += 200; p->boss_dmg += 0.5f; p->dmg_percent += 0.2f; p->ied += 0.1f; }
            p->weapon_tier++;
        } else if (recipe.craft_type == CRAFT_TYPE_ARMOR) {
            if (recipe.target_tier == 1) { p->max_hp += 500; p->max_mp += 200; }
            else if (recipe.target_tier == 2) { p->max_hp += 2000; p->max_mp += 1000; p->dmg_percent += 0.05f; }
            else if (recipe.target_tier == 3) { p->max_hp += 6000; p->max_mp += 3000; p->dmg_percent += 0.15f; }
            else if (recipe.target_tier == 4) { p->max_hp += 20000; p->max_mp += 10000; p->dmg_percent += 0.30f; p->boss_dmg += 0.1f; }
            p->armor_tier++;
        } else if (recipe.craft_type == CRAFT_TYPE_ACCESSORY) {
            if (recipe.target_tier == 1) { p->dex += 20; p->ied += 0.15f; }
            else if (recipe.target_tier == 2) { p->str += 50; p->dex += 50; p->intel += 50; p->luk += 50; p->ied += 0.30f; }
            else if (recipe.target_tier == 3) { p->str += 150; p->dex += 150; p->intel += 150; p->luk += 150; p->ied += 0.20f; }
            else if (recipe.target_tier == 4) { p->str += 400; p->dex += 400; p->intel += 400; p->luk += 400; p->ied += 0.20f; p->dmg_percent += 0.1f; }
            p->accessory_tier++;
        }
        printf("장비 등급이 Tier %d로 상승했습니다!\n", recipe.target_tier);

    } else {
        printf("\n💀 [실패] 제작에 실패했습니다... 💀\n");
        printf("일부 재료가 파손되었는지 확인합니다 (손실 확률: %d%%)\n", recipe.failure_loss_rate);
        
        for (int i = 0; i < recipe.material_count; i++) {
            consume_materials_logic(p, recipe.materials[i].material_name, recipe.materials[i].count, recipe.failure_loss_rate, 0);
        }
    }
}

void open_crafting_menu(Player* p) {
    int choice;
    while (1) {
        clear_screen();
        show_compact_status(p);
        printf("\n========= [마을 대장간] =========\n");
        printf("몬스터의 전리품을 모아 장비를 제작할 수 있습니다.\n\n");
        
        printf("1. 무기 제작 (현재 Tier %d)\n", p->weapon_tier);
        printf("2. 방어구 제작 (현재 Tier %d)\n", p->armor_tier);
        printf("3. 장신구 제작 (현재 Tier %d)\n", p->accessory_tier);
        printf("0. 나가기\n");
        printf("선택: ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 0) break;

        Recipe recipe;
        int target_type = choice;
        int current_tier = 0;
        if (target_type == 1) current_tier = p->weapon_tier;
        else if (target_type == 2) current_tier = p->armor_tier;
        else if (target_type == 3) current_tier = p->accessory_tier;

        if (current_tier >= 4) {
            printf("\n[알림] 이미 최고 등급의 제작법만 남았습니다 (또는 제작 한도 도달).\n");
            wait_for_enter();
            continue;
        }

        recipe.craft_type = target_type;
        recipe.target_tier = current_tier + 1;
        recipe.material_count = 0;

        // 레시피 설정
        if (recipe.target_tier == 1) {
            recipe.success_rate = 90;
            recipe.failure_loss_rate = 20;
            strcpy(recipe.materials[0].material_name, "달팽이 껍질"); recipe.materials[0].count = 5;
            strcpy(recipe.materials[1].material_name, "슬라임의 핵"); recipe.materials[1].count = 3;
            recipe.material_count = 2;
        } else if (recipe.target_tier == 2) {
            recipe.success_rate = 70;
            recipe.failure_loss_rate = 40;
            strcpy(recipe.materials[0].material_name, "고블린의 몽둥이"); recipe.materials[0].count = 5;
            strcpy(recipe.materials[1].material_name, "오크의 이빨"); recipe.materials[1].count = 3;
            strcpy(recipe.materials[2].material_name, "와일드카고의 눈동자"); recipe.materials[2].count = 1;
            recipe.material_count = 3;
        } else if (recipe.target_tier == 3) {
            recipe.success_rate = 50;
            recipe.failure_loss_rate = 70;
            strcpy(recipe.materials[0].material_name, "부서진 뼈"); recipe.materials[0].count = 5;
            strcpy(recipe.materials[1].material_name, "검은 화살촉"); recipe.materials[1].count = 3;
            strcpy(recipe.materials[2].material_name, "해적의 동전"); recipe.materials[2].count = 1;
            recipe.material_count = 3;
        } else if (recipe.target_tier == 4) {
            recipe.success_rate = 30;
            recipe.failure_loss_rate = 90;
            strcpy(recipe.materials[0].material_name, "와이번의 발톱"); recipe.materials[0].count = 5;
            strcpy(recipe.materials[1].material_name, "단단한 비늘"); recipe.materials[1].count = 3;
            strcpy(recipe.materials[2].material_name, "용의 뼈"); recipe.materials[2].count = 1;
            recipe.material_count = 3;
        }

        printf("\n--- 레시피 [Tier %d] ---\n", recipe.target_tier);
        for (int i = 0; i < recipe.material_count; i++) {
            int owned = get_material_count(p, recipe.materials[i].material_name);
            printf("- %s: %d / %d\n", recipe.materials[i].material_name, owned, recipe.materials[i].count);
        }
        printf("성공 확률: %d%% | 실패 시 재료 손실 확률: %d%%\n", recipe.success_rate, recipe.failure_loss_rate);
        
        printf("\n1. 제작 시작\n0. 취소\n선택: ");
        int craft_choice;
        scanf("%d", &craft_choice);
        clear_input_buffer();
        
        if (craft_choice == 1) {
            attempt_craft(p, recipe);
            wait_for_enter();
        }
    }
}
