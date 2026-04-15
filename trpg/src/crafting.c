#include "../include/crafting.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 재료 보유량 확인 함수
static int get_material_count(Player* p, const char* name) {
    int count = 0;
    for (int i = 0; i < p->inventory.count; i++) {
        if (strcmp(p->inventory.items[i].name, name) == 0) {
            count++;
        }
    }
    return count;
}

// 재료 소비 함수 (loss_rate 확률로 소실)
static void consume_materials_logic(Player* p, const char* name, int required, int loss_rate, int is_success) {
    int consumed = 0;
    for (int i = 0; i < p->inventory.count && consumed < required; ) {
        if (strcmp(p->inventory.items[i].name, name) == 0) {
            int should_remove = is_success || (rand() % 100 < loss_rate);
            
            if (should_remove) {
                for (int j = i; j < p->inventory.count - 1; j++) {
                    p->inventory.items[j] = p->inventory.items[j + 1];
                }
                p->inventory.count--;
                if (!is_success) printf("  - [소실] 재료 '%s'(을)를 잃었습니다.\n", name);
            } else {
                if (!is_success) printf("  - [보존] 재료 '%s'(은)는 무사합니다.\n", name);
                i++;
            }
            consumed++;
        } else {
            i++;
        }
    }
}

static void attempt_craft(Player* p, Recipe recipe) {
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
        
        for (int i = 0; i < recipe.material_count; i++) {
            consume_materials_logic(p, recipe.materials[i].material_name, recipe.materials[i].count, 100, 1);
        }

        if (recipe.craft_type == CRAFT_TYPE_WEAPON) {
            if (recipe.target_tier == 1) { printf(">> [거인의 철검] 제작! (보뎀 +5%%)\n"); p->boss_dmg += 0.05f; }
            else if (recipe.target_tier == 2) { printf(">> [저주받은 오닉스 블레이드] 제작! (보뎀 +15%%, 뎀퍼 +5%%)\n"); p->boss_dmg += 0.15f; p->dmg_percent += 0.05f; }
            else if (recipe.target_tier == 3) { printf(">> [드래곤 슬레이어] 제작! (보뎀 +30%%, 뎀퍼 +10%%)\n"); p->boss_dmg += 0.30f; p->dmg_percent += 0.10f; }
            else if (recipe.target_tier == 4) { printf(">> [갓 킬러] 제작! (보뎀 +60%%, 뎀퍼 +25%%)\n"); p->boss_dmg += 0.60f; p->dmg_percent += 0.25f; }
            p->c_weapon_tier++;
        } else if (recipe.craft_type == CRAFT_TYPE_ARMOR) {
            if (recipe.target_tier == 1) { printf(">> [늑대 가죽 망토] 제작! (뎀퍼 +5%%)\n"); p->dmg_percent += 0.05f; }
            else if (recipe.target_tier == 2) { printf(">> [바실리스크 스케일] 제작! (뎀퍼 +15%%, 방무 +5%%)\n"); p->dmg_percent += 0.15f; p->ied += 0.05f; }
            else if (recipe.target_tier == 3) { printf(">> [티탄의 판금 갑옷] 제작! (뎀퍼 +30%%, 방무 +15%%)\n"); p->dmg_percent += 0.30f; p->ied += 0.15f; }
            else if (recipe.target_tier == 4) { printf(">> [에테르넬 로브] 제작! (뎀퍼 +50%%, 방무 +30%%)\n"); p->dmg_percent += 0.50f; p->ied += 0.30f; }
            p->c_armor_tier++;
        } else if (recipe.craft_type == CRAFT_TYPE_ACCESSORY) {
            if (recipe.target_tier == 1) { printf(">> [독수리의 눈 반지] 제작! (방무 +10%%)\n"); p->ied += 0.10f; }
            else if (recipe.target_tier == 2) { printf(">> [메두사의 반지] 제작! (방무 +25%%, 보뎀 +5%%)\n"); p->ied += 0.25f; p->boss_dmg += 0.05f; }
            else if (recipe.target_tier == 3) { printf(">> [소울 스톤] 제작! (방무 +50%%, 보뎀 +15%%)\n"); p->ied += 0.50f; p->boss_dmg += 0.15f; }
            else if (recipe.target_tier == 4) { printf(">> [영원의 보석] 제작! (방무 +80%%, 보뎀 +30%%)\n"); p->ied += 0.80f; p->boss_dmg += 0.30f; }
            p->c_accessory_tier++;
        }
        printf("장비 등급이 Tier %d로 상승했습니다!\n", recipe.target_tier);
        update_combat_power(p);

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
        
        print_divider(80, YELLOW);
        print_centered(YELLOW_BG BLACK " [ ⚒️ 마을 대장간 - 장비 제작 ] " RESET, 80);
        print_divider(80, YELLOW);
        print_box_line("몬스터의 전리품을 모아 장비를 제작할 수 있습니다.", 80, YELLOW);
        print_divider(80, YELLOW);
        
        char buf[128];
        sprintf(buf, "1. 무기 연구/제작 (현재 Craft T%d)", p->c_weapon_tier);
        print_box_line(buf, 80, YELLOW);
        sprintf(buf, "2. 방어구 연구/제작 (현재 Craft T%d)", p->c_armor_tier);
        print_box_line(buf, 80, YELLOW);
        sprintf(buf, "3. 장신구 연구/제작 (현재 Craft T%d)", p->c_accessory_tier);
        print_box_line(buf, 80, YELLOW);
        print_box_line("0. 나가기", 80, YELLOW);
        print_divider(80, YELLOW);
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
        if (target_type == 1) current_tier = p->c_weapon_tier;
        else if (target_type == 2) current_tier = p->c_armor_tier;
        else if (target_type == 3) current_tier = p->c_accessory_tier;

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

        printf("\n");
        print_divider(80, YELLOW);
        char title_buf[64];
        sprintf(title_buf, "  레시피 [Tier %d]", recipe.target_tier);
        print_box_line(title_buf, 80, YELLOW);
        print_divider(80, YELLOW);
        for (int i = 0; i < recipe.material_count; i++) {
            int owned = get_material_count(p, recipe.materials[i].material_name);
            sprintf(buf, "  - %s: %d / %d", recipe.materials[i].material_name, owned, recipe.materials[i].count);
            print_box_line(buf, 80, YELLOW);
        }
        sprintf(buf, "  성공 확률: %d%% | 실패 시 재료 손실: %d%%", recipe.success_rate, recipe.failure_loss_rate);
        print_box_line(buf, 80, YELLOW);
        print_divider(80, YELLOW);
        
        printf("\n1. 제작 시작\n0. 취소\n선택: ");
        int craft_choice;
        if (scanf("%d", &craft_choice) != 1) { clear_input_buffer(); continue; }
        clear_input_buffer();
        
        if (craft_choice == 1) {
            attempt_craft(p, recipe);
            wait_for_enter();
        }
    }
}
