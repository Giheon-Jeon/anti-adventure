#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/combat.h"
#include "../include/utils.h"
#include "../include/generated_data.h"

// 주사위 정렬 (오름차순)
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

// n개의 주사위 굴리기
void roll_dice(int* dice, int count) {
    for (int i = 0; i < count; i++) {
        dice[i] = (rand() % 6) + 1;
    }
}

// 주사위를 시각적으로 출력 (박스 형태)
void print_dice_visual(int* dice, int count) {
    // 상단
    printf("  ");
    for (int i = 0; i < count; i++) printf("┌───┐  ");
    printf("\n");

    // 숫자
    printf("  ");
    for (int i = 0; i < count; i++) printf("│ %d │  ", dice[i]);
    printf("\n");

    // 하단
    printf("  ");
    for (int i = 0; i < count; i++) printf("└───┘  ");
    printf("\n");
}

static void print_combo_effect(const char* skill_name) {
    printf("\n" MAGENTA BOLD);
    printf("  ╔══════════════════════════════════════════════════════╗\n");
    printf("  ║                                                      ║\n");
    printf("  ║          🌟 액티브 콤보 스킬 발동!! 🌟               ║\n");
    printf("  ║                                                      ║\n");
    printf("  ║             >> %-20s <<             ║\n", skill_name);
    printf("  ║                                                      ║\n");
    printf("  ╚══════════════════════════════════════════════════════╝\n");
    printf(RESET "\n");
}

int is_combo_matched(Player* p, int dice[5], const char* skill_name) {
    int sum = 0;
    int prime_count = 0;
    int high_count = 0;
    int polar_count = 0;

    for (int i = 0; i < 5; i++) {
        sum += dice[i];
        if (dice[i] >= 4) high_count++;
        if (dice[i] == 2 || dice[i] == 3 || dice[i] == 5) prime_count++;
        if (dice[i] == 1 || dice[i] == 6) polar_count++;
    }

    if (strcmp(skill_name, "[진·용참선]") == 0) {
        return (high_count == 5);
    } else if (strcmp(skill_name, "[신·천궁살]") == 0) {
        return (sum == 15 || sum == 20);
    } else if (strcmp(skill_name, "[극·명도권]") == 0) {
        return (prime_count == 5);
    } else if (strcmp(skill_name, "[비·영격참]") == 0) {
        return (polar_count == 5);
    }
    return 0;
}

// 야추 규칙 데미지 계산
int calculate_yacht_damage(Player* p, int dice[5]) {
    int counts[7] = {0}; // 1~6 눈금 개수
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        counts[dice[i]]++;
        sum += dice[i];
    }

    // --- 콤보 스킬 우선 체크 ---
    for (int i = 0; i < p->skill_count; i++) {
        if (p->learned_skills[i].type == SKILL_TYPE_COMBO) {
            if (is_combo_matched(p, dice, p->learned_skills[i].name)) {
                print_combo_effect(p->learned_skills[i].name);
                return sum * 10 * (int)p->learned_skills[i].multiplier;
            }
        }
    }

    sort_dice(dice, 5);

    // 1. Yacht (5개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 5) {
            int mult = 30;
            const char* skill_name = "Yacht!";
            
            if (p->job == JOB_THIEF) { mult = 1000; skill_name = "데들리 럭!"; }
            else if (p->job == JOB_BERSERKER) { mult = 200; skill_name = "광란의 일격!"; }
            else if (p->job == JOB_ASSASSIN) { mult = 300; skill_name = "일격필살!"; }
            else if (p->job == JOB_JUDGE) { mult = 400; skill_name = "최후의 심판!"; }
            else if (p->job == JOB_AVATAR) { mult = 500; skill_name = "신성한 심판!"; }
            
            if (mult > 30) printf(YELLOW BOLD "[직업 스킬: %s (Yacht %d배)]\n" RESET, skill_name, mult);
            else printf(YELLOW BOLD "[조합: Yacht! (30배)]\n" RESET);
            
            return sum * 10 * mult;
        }
    }

    // 2. Large Straight (5개 연속)
    int is_large_straight = 1;
    for (int i = 0; i < 4; i++) {
        if (dice[i+1] != dice[i] + 1) {
            is_large_straight = 0;
            break;
        }
    }
    if (is_large_straight) {
        int mult = 25;
        const char* skill_name = "Large Straight!";
        
        if (p->job == JOB_MAGE) { mult = 150; skill_name = "메테오 스트라이크!"; }
        else if (p->job == JOB_RANGER) { mult = 80; skill_name = "화살 폭격!"; }
        else if (p->job == JOB_GRANDMAGE) { mult = 200; skill_name = "궁극의 지혜!"; }
        else if (p->job == JOB_CHAMPION) { mult = 100; skill_name = "영광의 길!"; }
        else if (p->job == JOB_AVATAR) { mult = 120; skill_name = "세상의 끝!"; }
        
        if (mult > 25) printf(CYAN BOLD "[직업 스킬: %s (L.Straight %d배)]\n" RESET, skill_name, mult);
        else printf(CYAN BOLD "[조합: Large Straight! (25배)]\n" RESET);
        return sum * 10 * mult;
    }

    // 3. Full House (3개 + 2개)
    int has_3 = 0, has_2 = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) has_3 = i;
        if (counts[i] == 2) has_2 = i;
    }
    if (has_3 && has_2) {
        int mult = 20;
        const char* skill_name = "Full House!";
        
        if (p->job == JOB_CRUSADER) { mult = 100; skill_name = "강화된 신성력!"; }
        else if (p->job == JOB_PALADIN) { mult = 150; skill_name = "성궤의 심판!"; }
        
        if (mult > 20) printf(YELLOW "[직업 스킬: %s (Full House %d배)]\n" RESET, skill_name, mult);
        else printf(YELLOW "[조합: Full House! (20배)]\n" RESET);
        return sum * 10 * mult;
    }

    // 4. Small Straight (4개 연속)
    int consecutive = 0, max_consecutive = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] > 0) consecutive++;
        else consecutive = 0;
        if (consecutive > max_consecutive) max_consecutive = consecutive;
    }
    if (max_consecutive >= 4) {
        int mult = 15;
        const char* skill_name = "Small Straight!";
        
        if (p->job == JOB_ARCHER) { mult = 45; skill_name = "샤프 아이즈!"; }
        else if (p->job == JOB_RANGER) { mult = 40; skill_name = "신속의 화살!"; }
        else if (p->job == JOB_CHAMPION) { mult = 60; skill_name = "용사의 연격!"; }
        else if (p->job == JOB_GRANDMAGE) { mult = 80; skill_name = "마력의 파동!"; }
        
        if (mult > 15) printf(CYAN "[직업 스킬: %s (S.Straight %d배)]\n" RESET, skill_name, mult);
        else printf(CYAN "[조합: Small Straight! (15배)]\n" RESET);
        return sum * 10 * mult;
    }

    // 5. Quad (4개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 4) {
            int mult = 10;
            const char* skill_name = "Quad!";
            
            if (p->job == JOB_BERSERKER) { mult = 50; skill_name = "무자비한 타격!"; }
            else if (p->job == JOB_ASSASSIN) { mult = 60; skill_name = "급소 찌르기!"; }
            else if (p->job == JOB_JUDGE) { mult = 80; skill_name = "철퇴 하강!"; }
            else if (p->job == JOB_PALADIN) { mult = 70; skill_name = "홀리 스트라이크!"; }
            
            if (mult > 10) printf(WHITE BOLD "[직업 스킬: %s (Quad %d배)]\n" RESET, skill_name, mult);
            else printf(WHITE BOLD "[조합: Quad! (10배)]\n" RESET);
            return (i * 4 * 10 * mult) + ((sum - i * 4) * 10);
        }
    }

    // 6. Triple (3개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) {
            int mult = 5;
            const char* skill_name = "Triple!";
            
            if (p->job == JOB_WARRIOR) { mult = 12; skill_name = "파이널 어택!"; }
            else if (p->job == JOB_GLADIATOR) { mult = 15; skill_name = "쌍검 난무!"; }
            else if (p->job == JOB_SAGE) { mult = 20; skill_name = "지혜의 일격!"; }
            
            if (mult > 5) printf(WHITE "[직업 스킬: %s (Triple %d배)]\n" RESET, skill_name, mult);
            else printf(WHITE "[조합: Triple! (5배)]\n" RESET);
            return (i * 3 * 10 * mult) + ((sum - i * 3) * 10);
        }
    }

    // 7. Pair (2개 동일)
    for (int i = 6; i >= 1; i--) {
        if (counts[i] == 2) {
            int mult = 2;
            if (p->job == JOB_GLADIATOR || p->job == JOB_SAGE) {
                printf(WHITE "[직업 스킬: 연쇄 반응! (Pair 6배)]\n" RESET);
                mult = 6;
            } else {
                printf(WHITE "[조합: One Pair! (2배)]\n" RESET);
            }
            return (i * 2 * 10 * mult) + ((sum - i * 2) * 10);
        }
    }

    // 8. No Set (기본)
    printf("[조합: No Set (기본)]\n");
    return sum * 10;
}

static MonsterBookEntry* get_monster_entry(Player* p, int monster_id) {
    if (monster_id <= 0) return NULL;
    for (int i = 0; i < MAX_BOOK_ENTRIES; i++) {
        if (p->encyclopedia[i].id == monster_id) return &p->encyclopedia[i];
        if (p->encyclopedia[i].id == 0) {
             p->encyclopedia[i].id = monster_id;
             p->encyclopedia[i].is_registered = 0;
             p->encyclopedia[i].kill_count = 0;
             return &p->encyclopedia[i];
        }
    }
    return NULL;
}

int calculate_final_damage(Player* p, Monster* m, int yacht_result) {
    float base_stat_power = 0;
    switch (p->job) {
        case JOB_WARRIOR: base_stat_power = (p->str * 5.0f); break;
        case JOB_ARCHER:  base_stat_power = (p->dex * 5.0f); break;
        case JOB_MAGE:    base_stat_power = (p->intel * 5.0f); break;
        case JOB_THIEF:   base_stat_power = (p->luk * 5.0f); break;
        case JOB_GLADIATOR: base_stat_power = (p->str * 3.0f + p->dex * 3.0f); break;
        case JOB_CRUSADER:  base_stat_power = (p->str * 3.0f + p->intel * 3.0f); break;
        case JOB_BERSERKER: base_stat_power = (p->str * 3.0f + p->luk * 3.0f); break;
        case JOB_RANGER:    base_stat_power = (p->dex * 3.0f + p->intel * 3.0f); break;
        case JOB_ASSASSIN:  base_stat_power = (p->dex * 3.0f + p->luk * 3.0f); break;
        case JOB_SAGE:      base_stat_power = (p->intel * 3.0f + p->luk * 3.0f); break;
        case JOB_CHAMPION:  base_stat_power = (p->str * 2.5f + p->dex * 2.5f + p->intel * 2.5f); break;
        case JOB_JUDGE:     base_stat_power = (p->str * 2.5f + p->dex * 2.5f + p->luk * 2.5f); break;
        case JOB_PALADIN:   base_stat_power = (p->str * 2.5f + p->intel * 2.5f + p->luk * 2.5f); break;
        case JOB_GRANDMAGE: base_stat_power = (p->dex * 2.5f + p->intel * 2.5f + p->luk * 2.5f); break;
        case JOB_AVATAR:    base_stat_power = (p->str + p->dex + p->intel + p->luk) * 2.0f; break;
        default:          base_stat_power = (p->str * 4.0f + p->dex); break;
    }
    
    float skill_extra_mult = 0.0f;
    int skill_extra_atk = 0;
    
    for (int i = 0; i < p->skill_count; i++) {
        skill_extra_mult += p->learned_skills[i].multiplier * p->learned_skills[i].level;
        skill_extra_atk += p->learned_skills[i].base_atk_bonus * p->learned_skills[i].level;
    }
    
    if (p->has_ultimate) {
        skill_extra_mult += p->ultimate_skill.multiplier;
        skill_extra_atk += p->ultimate_skill.base_atk_bonus;
    }

    float final_skill_mult = (yacht_result / 100.0f) + skill_extra_mult; 
    float damage = (base_stat_power + skill_extra_atk) * final_skill_mult;
    damage *= (1.0f + p->dmg_percent);
    if (m->is_boss) damage *= (1.0f + p->boss_dmg);

    // 도감 보너스 (연구 레벨 2: 10회 이상 처치 시 데미지 20% 증가)
    MonsterBookEntry* entry = get_monster_entry(p, m->id);
    if (entry && entry->is_registered && entry->kill_count >= 10) {
        damage *= 1.2f;
    }

    float effective_def = m->def * (1.0f - p->ied);
    if (effective_def < 0) effective_def = 0;
    int final_dmg = (int)(damage - effective_def);
    return (final_dmg < 1) ? 1 : final_dmg;
}

void start_combat(Player* p, Dungeon* d) {
    Monster enemy;
    int roll = rand() % 100;
    int enemy_rank = 0;

    // 몬스터 정보 결정 (던전에 따라 다름)
    if (d->has_boss && roll >= 80) { // 20%확률 보스
        enemy = d->boss;
    } else {
        int idx = rand() % d->monster_count;
        enemy = d->monsters[idx];
    }
    
    // 던전 수준에 따른 랭크 (보상용)
    if (d->min_cp < 500) enemy_rank = 0;
    else if (d->min_cp < 2000) enemy_rank = 1;
    else if (d->min_cp < 5000) enemy_rank = 2;
    else enemy_rank = 3;

    enemy.hp = enemy.max_hp;

    clear_screen();
    print_divider(60, CYAN);
    printf(BOLD);
    print_centered(d->name, 60);
    print_centered("던전에 입장했습니다!", 60);
    printf(RESET);
    print_divider(60, CYAN);
    
    printf("\n" RED BOLD);
    print_centered("⚠️ 몬스터 출현! ⚠️", 60);
    printf(RESET);
    printf(RED "\n      [%s] (방어율: %d)\n" RESET, enemy.name, enemy.def);
    
    // 자동 사냥 체크 (연구 레벨 3: 30회 이상 처치 시)
    int skip_combat = 0;
    MonsterBookEntry* entry = get_monster_entry(p, enemy.id);
    if (!enemy.is_boss && entry && entry->is_registered && entry->kill_count >= 30) {
        printf("\n" YELLOW BOLD "[도감 마스터] %s의 모든 패턴을 파악했습니다!" RESET "\n", enemy.name);
        printf(" 1. 일반 전투\n 2. 자동 사냥 (즉시 처치)\n 선택: ");
        int auto_choice;
        if (scanf("%d", &auto_choice) == 1 && auto_choice == 2) {
            printf(GREEN BOLD "\n✨ 순식간에 %s를 처치했습니다! ✨\n" RESET, enemy.name);
            enemy.hp = 0;
            skip_combat = 1;
        }
        clear_input_buffer();
    }

    if (!skip_combat) wait_for_enter();

    while (p->hp > 0 && enemy.hp > 0) {
        clear_screen();
        print_divider(60, CYAN);
        draw_hp_bar("PLAYER", p->hp, p->max_hp, 30, GREEN);
        draw_hp_bar(enemy.name, enemy.hp, enemy.max_hp, 30, RED);
        print_divider(60, CYAN);
        
        int choice;
        printf("\n" BOLD "[플레이어 턴]" RESET "\n");
        printf(" 1. 공격하기\n 2. 도망가기\n 선택: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 2) {
            int escape_dice[5];
            roll_dice(escape_dice, 5);
            sort_dice(escape_dice, 5);
            int sum = 0;
            for(int i=0; i<5; i++) sum += escape_dice[i];

            printf("\n" CYAN ">> 도망 시도! " RESET "\n");
            print_dice_visual(escape_dice, 5);
            printf(" (합계: %d)\n", sum);

            if (sum >= 15) {
                printf(GREEN BOLD "\n✨ 무사히 도망쳤습니다! ✨\n" RESET);
                wait_for_enter();
                return;
            } else {
                printf(RED BOLD "\n💀 도망에 실패했습니다! 공격 기회를 잃었습니다... 💀\n" RESET);
            }
        } else {
            int p_dice[5];
            roll_dice(p_dice, 5);
            sort_dice(p_dice, 5);
            printf("\n" CYAN ">> 공격 개시! " RESET "\n");
            print_dice_visual(p_dice, 5);
            
            int yacht_score = calculate_yacht_damage(p, p_dice);
            int final_dmg = calculate_final_damage(p, &enemy, yacht_score);
            
            printf(YELLOW BOLD "\n>> TOTAL DAMAGE: [ %d ]\n" RESET, final_dmg);
            enemy.hp -= final_dmg;

            if (enemy.hp > 0) {
                float double_attack_prob = 0;
                for (int i = 0; i < ABILITY_COUNT; i++) {
                    if (p->abilities[i].type == ABILITY_TYPE_DOUBLE_ATTACK) {
                        double_attack_prob += p->abilities[i].value;
                    }
                }
                if (rand() % 100 < (int)double_attack_prob) {
                    printf("\n" YELLOW BOLD "✨ [어빌리티] 연속 공격 발동! ✨" RESET "\n");
                    int p_dice2[5];
                    roll_dice(p_dice2, 5);
                    sort_dice(p_dice2, 5);
                    print_dice_visual(p_dice2, 5);
                    int yacht_score2 = calculate_yacht_damage(p, p_dice2);
                    int final_dmg2 = calculate_final_damage(p, &enemy, yacht_score2);
                    printf(YELLOW BOLD ">> ADDITIONAL DAMAGE: [ %d ]\n" RESET, final_dmg2);
                    enemy.hp -= final_dmg2;
                }
            }

            if (enemy.hp <= 0) {
                enemy.hp = 0;
                int exp_gain = (enemy_rank + 1) * 100;
                int gold_gain = (enemy_rank + 1) * 200;
                if (enemy.is_boss) {
                    exp_gain *= 5; gold_gain *= 10;
                    printf(YELLOW BOLD "\n🏆 보물 같은 보스 처치! 🏆\n" RESET);
                }
                p->exp += exp_gain;
                p->gold += gold_gain;
                
                printf(GREEN BOLD "\n🎯 승리! " RESET "EXP +%d, Gold +%d 획득!\n", exp_gain, gold_gain);
                
                if (p->inventory.count < MAX_INVENTORY_SIZE) {
                    p->inventory.items[p->inventory.count] = enemy.loot;
                    p->inventory.count++;
                    printf(YELLOW "🎁 전리품 획득: [%s] 🎁\n" RESET, enemy.loot.name);
                }

                // 도감 업데이트
                MonsterBookEntry* v_entry = get_monster_entry(p, enemy.id);
                if (v_entry) {
                    v_entry->kill_count++;
                    if (!v_entry->is_registered) {
                        int reg_chance = enemy.is_boss ? 5 : 10; // 보스 5%, 일반 10%
                        if (rand() % 100 < reg_chance) {
                            v_entry->is_registered = 1;
                            printf(MAGENTA BOLD "\n📖 [도감 등록] %s의 정보를 도감에 기록했습니다! 📖\n" RESET, enemy.name);
                        }
                    }
                }

                check_level_up(p);
                wait_for_enter();
                return;
            }
        }

        // 적 턴
        printf("\n" RED BOLD "[%s의 턴]" RESET " 공격합니다!\n", enemy.name);
        int e_dice[3];
        roll_dice(e_dice, 3);
        int e_dmg = (e_dice[0] + e_dice[1] + e_dice[2]) * (enemy_rank + 1) * 2;
        if (enemy.is_boss) e_dmg *= 3;
        
        printf(RED ">> %d의 데미지를 입었습니다!\n" RESET, e_dmg);
        p->hp -= e_dmg;
        if (p->hp <= 0) {
            p->hp = 0;
            printf(RED BOLD "\n💀 눈앞이 캄캄해졌습니다... 💀\n" RESET);
            apply_death_penalty(p);
            wait_for_enter();
            break;
        }
        wait_for_enter();
    }
}

void select_dungeon(Player* p) {
    Dungeon dungeons[DUNGEON_COUNT];
    
    for (int i = 0; i < DUNGEON_COUNT; i++) {
        const DungeonTemplate* t = &g_dungeon_templates[i];
        strcpy(dungeons[i].name, t->name);
        dungeons[i].min_cp = t->min_cp;
        dungeons[i].max_cp = t->max_cp;
        dungeons[i].monster_count = t->monster_count;
        for (int j = 0; j < t->monster_count; j++) {
            dungeons[i].monsters[j] = get_monster_by_id(t->monster_ids[j]);
        }
        dungeons[i].has_boss = t->has_boss;
        if (t->has_boss) {
            dungeons[i].boss = get_monster_by_id(t->boss_id);
        }
    }

    while(1) {
        clear_screen();
        show_compact_status(p);
        printf("\n========= [사냥터 선택] =========\n");
        printf("현재 전투력에 맞는 던전을 선택하세요.\n\n");
        
        for(int i=0; i<DUNGEON_COUNT; i++) {
            printf("%d. %s (권장 CP: %d ~ ", i+1, dungeons[i].name, dungeons[i].min_cp);
            if(dungeons[i].max_cp > 100000) printf("MAX)\n");
            else printf("%d)\n", dungeons[i].max_cp);
            printf("   - 등장: ");
            for(int j=0; j<dungeons[i].monster_count; j++) {
                printf("%s%s", dungeons[i].monsters[j].name, (j < dungeons[i].monster_count - 1) ? ", " : "");
            }
            if(dungeons[i].has_boss) printf(", %s", dungeons[i].boss.name);
            printf("\n");
        }
        printf("0. 뒤로 가기\n");
        printf("선택: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        if (choice == 0) break;
        if (choice < 1 || choice > DUNGEON_COUNT) {
            printf("[알림] 잘못된 선택입니다.\n");
            wait_for_enter();
            continue;
        }
        
        Dungeon *selected = &dungeons[choice-1];
        if (p->combat_power < selected->min_cp) {
            printf("\n⚠️ [경고] 전투력이 권장 수치보다 낮습니다! 진입하시겠습니까?\n");
            printf("1. 예 (위험 감수)\n2. 아니오 (마을로 돌아감)\n선택: ");
            int confirm;
            scanf("%d", &confirm);
            clear_input_buffer();
            if (confirm != 1) continue;
        }
        
        // 던전 진입 (단순하게 전투 바로 시작 또는 이벤트 발생)
        // 여기서는 기존 이벤트 시스템과 통합하거나 바로 start_combat 호출
        // 사용자가 "전투하러 가면... 알려줘"라고 했으므로 바로 전투 또는 던전 전용 이벤트
        start_combat(p, selected);
        break;
    }
}
