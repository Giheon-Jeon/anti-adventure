#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/combat.h"
#include "../include/utils.h"
#include "../include/generated_data.h"
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #define Sleep(ms) usleep((ms) * 1000)
#endif

// --- 주사위 유틸리티 ---

void sort_dice(int* dice, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (dice[i] > dice[j]) {
                int temp = dice[i];
                dice[i] = dice[j];
                dice[j] = temp;
            }
        }
    }
}

void roll_dice(int* dice, int count) {
    for (int i = 0; i < count; i++) {
        dice[i] = (rand() % 6) + 1;
    }
}

static void print_dice_visual(int* dice, int count) {
    printf("\n");
    char* pad = "            ";
    
    printf("%s", pad);
    for (int i = 0; i < count; i++) printf("+-------+ ");
    printf("\n%s", pad);
    for (int i = 0; i < count; i++) printf("|   %d   | ", dice[i]);
    printf("\n%s", pad);
    for (int i = 0; i < count; i++) printf("+-------+ ");
    printf("\n");
}

static void print_combo_effect(const char* skill_name) {
    printf("\n" MAGENTA BOLD "  >> 콤보 스킬 발동!! [%s] <<\n" RESET, skill_name);
}

// --- 콤보 매칭 ---

static int is_combo_matched(Player* p, int dice[5], const char* skill_name) {
    (void)p; // 미래 확장용 매개변수
    int sum = 0, prime_count = 0, high_count = 0, polar_count = 0;
    for (int i = 0; i < 5; i++) {
        sum += dice[i];
        if (dice[i] >= 4) high_count++;
        if (dice[i] == 2 || dice[i] == 3 || dice[i] == 5) prime_count++;
        if (dice[i] == 1 || dice[i] == 6) polar_count++;
    }
    if (strcmp(skill_name, "[진·용참선]") == 0) return (high_count == 5);
    else if (strcmp(skill_name, "[신·천궁살]") == 0) return (sum == 15 || sum == 20);
    else if (strcmp(skill_name, "[극·명왕권]") == 0) return (prime_count == 5);
    else if (strcmp(skill_name, "[비·영격참]") == 0) return (polar_count == 5);
    return 0;
}

// --- Yacht 데미지 계산 ---

int calculate_yacht_damage(Player* p, int dice[5]) {
    int counts[7] = {0};
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        counts[dice[i]]++;
        sum += dice[i];
    }

    // 콤보 스킬 체크
    for (int i = 0; i < p->skill_count; i++) {
        if (p->learned_skills[i].type == SKILL_TYPE_COMBO) {
            if (is_combo_matched(p, dice, p->learned_skills[i].name)) {
                print_combo_effect(p->learned_skills[i].name);
                return sum * 10 * (int)p->learned_skills[i].multiplier;
            }
        }
    }

    sort_dice(dice, 5);

    // Yacht (5개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 5) {
            int mult = 50;
            if (p->job == JOB_THIEF) mult = 150;
            else if (p->job == JOB_BERSERKER) mult = 100;
            printf(YELLOW BOLD "[조합: Yacht! (%d배)]\n" RESET, mult);
            return sum * 10 * mult;
        }
    }

    // Large Straight (5연속)
    int is_l_str = 1;
    for (int i = 0; i < 4; i++) if (dice[i+1] != dice[i] + 1) is_l_str = 0;
    if (is_l_str) {
        int mult = 30;
        if (p->job == JOB_MAGE) mult = 80;
        printf(CYAN BOLD "[조합: L.Straight (%d배)]\n" RESET, mult);
        return sum * 10 * mult;
    }

    // Full House (3+2)
    int has_3 = 0, has_2 = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) has_3 = 1;
        if (counts[i] == 2) has_2 = 1;
    }
    if (has_3 && has_2) {
        printf(YELLOW "[조합: Full House (25배)]\n" RESET);
        return sum * 10 * 25;
    }

    // Small Straight (4연속)
    int con = 0, max_con = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] > 0) con++;
        else con = 0;
        if (con > max_con) max_con = con;
    }
    if (max_con >= 4) {
        printf(CYAN "[조합: S.Straight (20배)]\n" RESET);
        return sum * 10 * 20;
    }

    // Four of a Kind
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 4) return (i * 4 * 10 * 15) + ((sum - i * 4) * 10);
    }
    // Three of a Kind
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) return (i * 3 * 10 * 10) + ((sum - i * 3) * 10);
    }
    // Pair
    for (int i = 6; i >= 1; i--) {
        if (counts[i] == 2) return (i * 2 * 10 * 5) + ((sum - i * 2) * 10);
    }

    return sum * 10;
}

// --- 도감 연구 보너스 계산 ---
static float get_encyclopedia_bonus(Player* p, int monster_id) {
    for (int i = 0; i < MAX_BOOK_ENTRIES; i++) {
        if (p->encyclopedia[i].id == monster_id && p->encyclopedia[i].is_registered) {
            if (p->encyclopedia[i].kill_count >= 10) {
                return 0.20f; // 연구 Lv.2 이상: +20% 데미지
            }
        }
    }
    return 0.0f;
}

// --- DOUBLE_ATTACK 어빌리티 확률 체크 ---
static int check_double_attack(Player* p) {
    for (int i = 0; i < ABILITY_COUNT; i++) {
        if (p->abilities[i].type == ABILITY_TYPE_DOUBLE_ATTACK) {
            int chance = (int)p->abilities[i].value;
            if (rand() % 100 < chance) return 1;
        }
    }
    return 0;
}

// --- 최종 데미지 계산 ---

int calculate_final_damage(Player* p, Monster* m, int yacht_result) {
    float base_stat_power = 0;
    switch (p->job) {
        case JOB_WARRIOR: base_stat_power = (p->str * 5.0f); break;
        case JOB_ARCHER:  base_stat_power = (p->dex * 5.0f); break;
        case JOB_MAGE:    base_stat_power = (p->intel * 5.0f); break;
        case JOB_THIEF:   base_stat_power = (p->luk * 5.0f); break;
        default:          base_stat_power = (p->str * 4.0f + p->dex); break;
    }
    
    // 무기 내구도 보정
    float w_dur = (p->weapon_dur > p->c_weapon_dur) ? p->weapon_dur : p->c_weapon_dur;
    float w_eff = 0.1f + 0.9f * (w_dur / 100.0f);
    base_stat_power *= w_eff;

    // 스킬 배율 합산
    float skill_extra_mult = 0.0f;
    for (int i = 0; i < p->skill_count; i++) {
        skill_extra_mult += p->learned_skills[i].multiplier * p->learned_skills[i].level;
    }

    float final_skill_mult = (yacht_result / 100.0f) + skill_extra_mult; 
    float damage = base_stat_power * final_skill_mult;
    
    // 도감 연구 보너스 적용
    float ency_bonus = get_encyclopedia_bonus(p, m->id);
    if (ency_bonus > 0) {
        damage *= (1.0f + ency_bonus);
    }
    
    // 몬스터 방어력 감산
    float effective_m_def = m->def * 1.0f;
    float m_reduction = 100.0f / (100.0f + effective_m_def);
    int final_dmg = (int)(damage * m_reduction);
    
    return (final_dmg < 1) ? 1 : final_dmg;
}

// --- 플레이어 피격 데미지 ---

int calculate_player_taken_damage(Player* p, int raw_enemy_atk) {
    int p_base_def = p->armor_tier * 30 + p->c_armor_tier * 50;
    
    float a_dur = (p->armor_dur > p->c_armor_dur) ? p->armor_dur : p->c_armor_dur;
    float a_eff = 0.1f + 0.9f * (a_dur / 100.0f);
    float effective_p_def = p_base_def * a_eff;

    float p_reduction = 100.0f / (100.0f + effective_p_def);
    int final_taken = (int)(raw_enemy_atk * p_reduction);
    return (final_taken < 1) ? 1 : final_taken;
}

// --- 도감 업데이트 ---
static void update_encyclopedia(Player* p, int monster_id) {
    // 기존 엔트리 검색
    for (int i = 0; i < MAX_BOOK_ENTRIES; i++) {
        if (p->encyclopedia[i].id == monster_id) {
            p->encyclopedia[i].kill_count++;
            return;
        }
    }
    // 새 엔트리 등록 (빈 슬롯 찾기)
    for (int i = 0; i < MAX_BOOK_ENTRIES; i++) {
        if (p->encyclopedia[i].id == 0) {
            p->encyclopedia[i].id = monster_id;
            p->encyclopedia[i].is_registered = 1;
            p->encyclopedia[i].kill_count = 1;
            printf(GREEN "\n📖 [도감 등록] 새로운 몬스터가 도감에 기록되었습니다!\n" RESET);
            return;
        }
    }
}

// --- 전리품 획득 ---
static void grant_loot(Player* p, Monster* m) {
    if (strlen(m->loot.name) == 0) return;
    if (p->inventory.count >= MAX_INVENTORY_SIZE) {
        printf(YELLOW "\n[인벤토리 가득] 전리품 '%s'을 버릴 수밖에 없었습니다...\n" RESET, m->loot.name);
        return;
    }
    
    Item loot = m->loot;
    loot.type = ITEM_TYPE_TROPHY;
    sprintf(loot.description, "%s의 전리품", m->name);
    
    p->inventory.items[p->inventory.count] = loot;
    p->inventory.count++;
    printf(GREEN "🎁 [전리품 획득] %s (%d G)\n" RESET, loot.name, loot.value);
}

// --- 전투 메인 루프 ---

void start_combat(Player* p, Dungeon* d) {
    Monster enemy;
    int roll = rand() % 100;
    int enemy_rank = (d->min_cp / 500);

    if (d->has_boss && roll >= 80) enemy = d->boss;
    else enemy = d->monsters[rand() % d->monster_count];
    
    enemy.hp = enemy.max_hp;
    clear_screen();
    printf("\n" RED BOLD "--- 전투 개시: %s (HP:%d) ---\n" RESET, enemy.name, enemy.hp);

    int turn = 0;
    while (p->hp > 0 && enemy.hp > 0) {
        turn++;
        clear_screen();
        
        char header_buf[128];
        sprintf(header_buf, "\n" RED BOLD "--- 전투 진행 중: %s ---" RESET, enemy.name);
        print_centered(header_buf, 80);
        
        print_divider(80, CYAN);
        
        char turn_buf[64];
        sprintf(turn_buf, BOLD "[ 제 %d 턴 ]" RESET, turn);
        print_centered(turn_buf, 80);
        
        // 상단 상태 정보
        draw_hp_bar(p->name, p->hp, p->max_hp, 35, GREEN); printf("\n");
        draw_hp_bar(enemy.name, enemy.hp, enemy.max_hp, 35, RED); printf("\n");
        print_divider(80, CYAN);
        
        wait_for_enter();
        
        // --- [플레이어의 공격] ---
        int p_dice[5];
        roll_dice(p_dice, 5);
        print_dice_visual(p_dice, 5);
        int yacht_score = calculate_yacht_damage(p, p_dice);
        int final_dmg = calculate_final_damage(p, &enemy, yacht_score);
        
        // DOUBLE_ATTACK 어빌리티 체크
        int is_double = check_double_attack(p);
        if (is_double) {
            final_dmg *= 2;
            printf(MAGENTA BOLD "\n  ⚡ [연속 공격!] 두 배의 힘으로 공격합니다!" RESET "\n");
        }
        
        printf(YELLOW "\n ⚔️  당신의 공격! %s에게 " RESET RED BOLD "%d" RESET YELLOW "의 대미지를 입혔습니다!" RESET "\n", enemy.name, final_dmg);
        enemy.hp -= (enemy.hp < final_dmg) ? enemy.hp : final_dmg;

        // 무기 내구도 소모
        if (p->weapon_dur > 0) p->weapon_dur--; 
        if (p->c_weapon_dur > 0) p->c_weapon_dur--;

        if (enemy.hp <= 0) {
            Sleep(800);
            
            // 보상 계산 (몬스터 스탯 기반)
            int exp_gain = (enemy.max_hp / 50) + (enemy.def * 2);
            int gold_gain = (enemy.max_hp / 20) + (rand() % 120) + 30;
            p->exp += exp_gain;
            p->gold += gold_gain;
            
            // 도감 업데이트 & 전리품 획득
            update_encyclopedia(p, enemy.id);
            grant_loot(p, &enemy);
            
            clear_screen();
            print_divider(80, YELLOW);
            print_centered(YELLOW_BG BLACK "         ⚔️ 전투 결과 보고서 ⚔️         " RESET, 80);
            print_divider(80, YELLOW);
            
            char buf[128];
            sprintf(buf, "처치한 적: %s", enemy.name);
            print_box_line(buf, 80, YELLOW);
            sprintf(buf, "획득 보상: %d 골드 (Gold)", gold_gain);
            print_box_line(buf, 80, YELLOW);
            sprintf(buf, "획득 경험: %d EXP", exp_gain);
            print_box_line(buf, 80, YELLOW);
            
            // 도감 연구 보너스가 있으면 표시
            float ency_bonus = get_encyclopedia_bonus(p, enemy.id);
            if (ency_bonus > 0) {
                sprintf(buf, "📖 도감 연구 보너스: 데미지 +%.0f%%", ency_bonus * 100);
                print_box_line(buf, 80, GREEN);
            }
            
            print_divider(80, YELLOW);
            printf("\n" BOLD "성장 현황:" RESET "\n");
            int req_exp = get_required_exp(p->level);
            draw_exp_bar(p->exp, req_exp, 40); printf("\n");
            
            check_level_up(p);
            wait_for_enter();
            return;
        }

        Sleep(800);

        // --- [적의 공격] ---
        int raw_e_atk = (rand() % 20 + 10) * (enemy_rank + 1);
        if (enemy.is_boss) raw_e_atk *= 2;
        int taken_dmg = calculate_player_taken_damage(p, raw_e_atk);
        
        printf(RED "\n 👿  %s의 반격! 당신에게 " RESET RED BOLD "%d" RESET RED "의 대미지를 입혔습니다..." RESET "\n", enemy.name, taken_dmg);
        p->hp -= (p->hp < taken_dmg) ? p->hp : taken_dmg;

        // 방어구 내구도 소모
        if (p->armor_dur > 0) p->armor_dur--;
        if (p->c_armor_dur > 0) p->c_armor_dur--;

        if (p->hp <= 0) {
            Sleep(1000);
            apply_death_penalty(p);
            return;
        }

        wait_for_enter();
    }
}

// --- 던전 데이터 로드 ---

Dungeon get_dungeon_data(int index) {
    Dungeon d = {0};
    if (index < 0 || index >= DUNGEON_COUNT) return d;
    
    const DungeonTemplate* t = &g_dungeon_templates[index];
    strncpy(d.name, t->name, 49);
    d.min_cp = t->min_cp;
    d.max_cp = t->max_cp;
    d.monster_count = 0;
    
    for (int i = 0; i < 3; i++) {
        if (t->monster_ids[i] > 0) {
            d.monsters[d.monster_count++] = get_monster_by_id(t->monster_ids[i]);
        }
    }
    
    if (t->has_boss && t->boss_id > 0) {
        d.boss = get_monster_by_id(t->boss_id);
        d.has_boss = 1;
    }
    
    return d;
}

// --- 던전 선택 (동적 메뉴 생성) ---

void select_dungeon(Player* p) {
    clear_screen();
    print_divider(80, CYAN);
    print_centered(BOLD "========= [던전 탐험 선택] =========" RESET, 80);
    print_divider(80, CYAN);

    // g_dungeon_templates[]에서 동적으로 메뉴 생성
    char buf[128];
    for (int i = 0; i < DUNGEON_COUNT; i++) {
        const DungeonTemplate* t = &g_dungeon_templates[i];
        const char* lock = (p->combat_power >= t->min_cp) ? GREEN "✓" RESET : RED "✗" RESET;
        sprintf(buf, "%d. %s %s (요구 CP: %d)", i + 1, lock, t->name, t->min_cp);
        print_box_line(buf, 80, CYAN);
    }
    print_box_line("0. 마을로 돌아가기", 80, CYAN);
    print_divider(80, CYAN);
    printf("선택: ");

    int choice;
    if (scanf("%d", &choice) != 1) { clear_input_buffer(); return; }
    clear_input_buffer();
    
    if (choice < 1 || choice > DUNGEON_COUNT) return;
    
    Dungeon selected = get_dungeon_data(choice - 1);

    if (p->combat_power < selected.min_cp) {
        printf(RED "\n[경고] 전투력이 부족하여 진입할 수 없습니다! (필요: %d)\n" RESET, selected.min_cp);
        wait_for_enter();
        return;
    }

    start_combat(p, &selected);
}
