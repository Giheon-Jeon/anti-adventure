#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/character.h"
#include "../include/utils.h"

#define ALL_SKILLS_COUNT 100

static Skill skill_db[ALL_SKILLS_COUNT];
static int skill_db_count = 0;

void add_to_db(int id, const char* name, const char* desc, float mult, int atk, int luk, int hp, int mp, int str, int dex, int intel, int cost, SkillType type, JobType job, int tier, int prereq, int is_p, int is_a) {
    if (skill_db_count >= ALL_SKILLS_COUNT) return;
    Skill* s = &skill_db[skill_db_count];
    s->id = id;
    strcpy(s->name, name);
    strcpy(s->description, desc);
    s->level = 0; // 초기 레벨은 0 (투자 전)
    s->max_level = MAX_SKILL_LEVEL;
    s->multiplier = mult;
    s->base_atk_bonus = atk;
    s->luk_bonus = luk;
    s->hp_bonus = hp;
    s->mp_bonus = mp;
    s->str_bonus = str;
    s->dex_bonus = dex;
    s->int_bonus = intel;
    s->mp_cost = cost;
    s->type = type;
    s->required_job = job;
    s->tier = tier;
    s->prereq_id = prereq;
    s->is_passive = is_p;
    s->is_active = is_a;
    skill_db_count++;
}

void init_skill_system() {
    skill_db_count = 0;
    // ID, 이름, 설명, 배수, 공증, 행운, HP, MP, STR, DEX, INT, 소모, 타입, 직업, 티어, 선행ID, 패시브, 액티브
    
    // --- 공통 스킬 (Tier 0) ---
    add_to_db(1, "기초 체력 단련", "영구적으로 공격력이 상승합니다.", 0.0f, 5, 0, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE, 0, -1, 1, 0);
    add_to_db(2, "집중력 강화", "주사위 데미지 배수가 상승합니다.", 0.1f, 0, 0, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE, 0, -1, 1, 0);
    add_to_db(3, "운수 좋은 날", "공격력과 행운(LUK)이 상승합니다.", 0.0f, 2, 5, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE, 0, 1, 1, 0); // 1번 5렙 필요(논리적으로)
    add_to_db(4, "강인한 신체", "최대 HP가 상승합니다.", 0.0f, 0, 0, 50, 0, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE, 0, 1, 1, 0);
    add_to_db(5, "끝없는 마력", "최대 MP가 상승합니다.", 0.0f, 0, 0, 0, 30, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE, 0, 1, 1, 0);

    // --- 전사 계열 (ID 11~) ---
    add_to_db(11, "파워 스트라이크", "전사의 기본 공격 스킬입니다.", 1.2f, 10, 0, 0, 0, 5, 0, 0, 10, SKILL_TYPE_JOB, JOB_WARRIOR, 1, -1, 0, 1);
    add_to_db(12, "가드 브레이크", "적의 방어를 무시하는 일격입니다.", 1.0f, 15, 0, 0, 0, 5, 0, 0, 15, SKILL_TYPE_JOB, JOB_WARRIOR, 1, 11, 1, 1); // 11번 5렙 필요
    add_to_db(13, "소드 댄스", "검투사의 화려한 연속 공격입니다.", 2.0f, 20, 0, 0, 0, 10, 5, 0, 25, SKILL_TYPE_JOB, JOB_GLADIATOR, 2, 11, 0, 1);
    add_to_db(14, "피의 갈증", "HP를 소모해 폭발적인 힘을 얻습니다.", 3.5f, 50, 0, -30, 0, 20, 0, 0, 0, SKILL_TYPE_JOB, JOB_BERSERKER, 2, 11, 1, 1);

    // --- 궁수 계열 (ID 21~) ---
    add_to_db(21, "에로우 블로우", "궁수의 기본 사격 스킬입니다.", 1.1f, 8, 0, 0, 0, 0, 5, 0, 8, SKILL_TYPE_JOB, JOB_ARCHER, 1, -1, 0, 1);
    add_to_db(22, "더블 샷", "두 발의 화살을 동시에 발사합니다.", 1.6f, 5, 0, 0, 0, 0, 8, 0, 12, SKILL_TYPE_JOB, JOB_ARCHER, 1, 21, 0, 1);
    add_to_db(23, "원드 애로우", "바람의 힘을 실은 사격입니다.", 2.2f, 15, 0, 0, 0, 0, 12, 10, 20, SKILL_TYPE_JOB, JOB_RANGER, 2, 21, 0, 1);

    // --- 마법사 계열 (ID 31~) ---
    add_to_db(31, "에너지 볼트", "마법사의 기본 마법 화살입니다.", 1.0f, 5, 0, 0, 10, 0, 0, 5, 10, SKILL_TYPE_JOB, JOB_MAGE, 1, -1, 0, 1);
    add_to_db(32, "매직 클로", "적을 할퀴는 마력의 손톱입니다.", 1.8f, 5, 0, 0, 0, 0, 0, 10, 15, SKILL_TYPE_JOB, JOB_MAGE, 1, 31, 0, 1);
    add_to_db(33, "이클립스", "거대한 암흑 폭발을 일으킵니다.", 5.0f, 80, 0, 0, 0, 0, 0, 50, 100, SKILL_TYPE_JOB, JOB_GRANDMAGE, 2, 31, 0, 1);

    // --- 도적 계열 (ID 41~) ---
    add_to_db(41, "럭키 세븐", "행운을 담은 두 개의 표창을 던집니다.", 2.0f, 0, 10, 0, 0, 0, 0, 0, 12, SKILL_TYPE_JOB, JOB_THIEF, 1, -1, 0, 1);
    add_to_db(42, "디스오더", "적을 혼란에 빠뜨립니다.", 1.1f, 10, 5, 0, 0, 0, 5, 0, 15, SKILL_TYPE_JOB, JOB_THIEF, 1, 41, 1, 1);
    add_to_db(43, "그림자 습격", "순식간에 적의 뒤를 노립니다.", 4.0f, 50, 20, 0, 0, 0, 20, 0, 35, SKILL_TYPE_JOB, JOB_ASSASSIN, 2, 41, 0, 1);

    // --- 콤보 스킬 (자동 습득용 IDs 80~) ---
    add_to_db(80, "[진·용참선]", "강력한 일격 (모든 주사위 4 이상)", 80.0f, 0, 0, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMBO, JOB_NONE, 0, -1, 0, 1);
    add_to_db(81, "[신·천궁살]", "백발백중 (주사위 합 15/20)", 100.0f, 0, 0, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMBO, JOB_NONE, 0, -1, 0, 1);
    add_to_db(82, "[극·명도권]", "마력 권능 (모든 주사위 소수)", 90.0f, 0, 0, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMBO, JOB_NONE, 0, -1, 0, 1);
    add_to_db(83, "[비·영격참]", "치명적 습격 (1과 6으로만 구성)", 120.0f, 0, 0, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMBO, JOB_NONE, 0, -1, 0, 1);
}

const char* get_skill_type_name(SkillType type) {
    switch (type) {
        case SKILL_TYPE_COMMON: return "공통";
        case SKILL_TYPE_JOB: return "직업";
        case SKILL_TYPE_ULTIMATE: return "궁극기";
        case SKILL_TYPE_COMBO: return "콤보";
        default: return "알 수 없음";
    }
}

void apply_skill_effects(Player* p) {
    // 이 함수는 전투 시작 전이나 스탯 갱신 시 호출됨
    // 실제 데미지 계산 로직(combat.c)에서 스킬 목록을 순회하여 사용함
}

void grant_event_skill(Player* p, const char* name, const char* desc, float mult, int atk, int luk, int hp, int mp, int str, int dex, int intel, int cost) {
    if (p->skill_count >= MAX_LEARNED_SKILLS) {
        printf("\n[알림] 스킬 칸이 부족하여 [%s] 스킬을 배울 수 없습니다.\n", name);
        return;
    }
    
    // 이미 배운 스킬인지 확인 (중복 방지)
    for (int i = 0; i < p->skill_count; i++) {
        if (strcmp(p->learned_skills[i].name, name) == 0) {
            // 이미 있으면 레벨업 처리
            p->learned_skills[i].level++;
            // 기존 스탯 보너스 재적용 (누적)
            p->max_hp += hp; p->hp += hp;
            p->max_mp += mp; p->mp += mp;
            p->str += str;
            p->dex += dex;
            p->intel += intel;
            p->luk += luk;
            printf("\n🌟 보상: [%s] 스킬이 LV.%d로 강화되었습니다! 🌟\n", name, p->learned_skills[i].level);
            return;
        }
    }

    Skill* s = &p->learned_skills[p->skill_count];
    strcpy(s->name, name);
    strcpy(s->description, desc);
    s->level = 1;
    s->multiplier = mult;
    s->base_atk_bonus = atk;
    s->luk_bonus = luk;
    s->hp_bonus = hp;
    s->mp_bonus = mp;
    s->str_bonus = str;
    s->dex_bonus = dex;
    s->int_bonus = intel;
    s->mp_cost = cost;
    s->type = SKILL_TYPE_COMMON; // 고유 스킬도 공통 타입으로 취급
    s->required_job = JOB_NONE;

    // 즉시 스탯 반영
    p->max_hp += hp; p->hp += hp;
    p->max_mp += mp; p->mp += mp;
    p->str += str;
    p->dex += dex;
    p->intel += intel;
    p->luk += luk;

    p->skill_count++;
    printf("\n✨ 특별 보상: 영구적인 고유 스킬 [%s]를 획득했습니다! ✨\n", name);
    printf("   - %s\n", desc);
}

void show_skill_tree(Player* p) {
    while (1) {
        clear_screen();
        int box_width = 76; // 더 안전한 너비로 조정
        char line[512];
        
        printf("\n" CYAN "┌"); for(int i=0; i<box_width-2; i++) printf("─"); printf("┐" RESET "\n");
        
        sprintf(line, "  [ 스킬 트리 메뉴 ] (보유 포인트: %d)", p->skill_points);
        int v_title = get_visual_width(line);
        printf(CYAN "│" RESET YELLOW BOLD "%s", line);
        for(int i=0; i < (box_width - 2 - v_title); i++) printf(" ");
        printf(CYAN "│" RESET "\n");
        
        printf(CYAN "├"); for(int i=0; i<box_width-2; i++) printf("─"); printf("┤" RESET "\n");
        
        sprintf(line, "  현재 직업: [ %s ]", get_job_name(p->job));
        int v_job = get_visual_width(line);
        printf(CYAN "│" RESET GREEN BOLD "%s", line);
        for(int i=0; i < (box_width - 2 - v_job); i++) printf(" ");
        printf(CYAN "│" RESET "\n");
        
        printf(CYAN "├"); for(int i=0; i<box_width-2; i++) printf("─"); printf("┤" RESET "\n");

        int visible_indices[ALL_SKILLS_COUNT];
        int visible_count = 0;
        for (int i = 0; i < skill_db_count; i++) {
            Skill* s = &skill_db[i];
            if (s->type == SKILL_TYPE_COMBO) continue;
            if (s->required_job == JOB_NONE || p->job == s->required_job) {
                visible_indices[visible_count++] = i;
            }
        }

        for (int i = 0; i < visible_count; i++) {
            int db_idx = visible_indices[i];
            Skill* s = &skill_db[db_idx];
            
            int learned_idx = -1;
            for (int k = 0; k < p->skill_count; k++) {
                if (p->learned_skills[k].id == s->id) { learned_idx = k; break; }
            }
            int current_lv = (learned_idx == -1) ? 0 : p->learned_skills[learned_idx].level;
            
            int is_locked = 0;
            if (s->prereq_id != -1) {
                int prereq_lv = 0;
                for (int k = 0; k < p->skill_count; k++) {
                    if (p->learned_skills[k].id == s->prereq_id) { prereq_lv = p->learned_skills[k].level; break; }
                }
                if (prereq_lv < 5) is_locked = 1;
            }

            if (is_locked) {
                sprintf(line, "  %2d. [X] %-18s (ID:%d LV.5 필요)", i + 1, s->name, s->prereq_id);
                int v_lock = get_visual_width(line);
                printf(CYAN "│" RESET RED "%s", line);
                for(int j=0; j < (box_width - 2 - v_lock); j++) printf(" ");
                printf(CYAN "│" RESET "\n");
            } else {
                const char* type_tag = (s->is_active && s->is_passive) ? "A+P" : (s->is_active ? "ACT" : "PAS");
                sprintf(line, "  %2d. [%-3s] %-18s (LV.%2d/%2d)", i+1, type_tag, s->name, current_lv, s->max_level);
                int v_skill = get_visual_width(line);
                printf(CYAN "│" RESET WHITE BOLD "%s", line);
                for(int j=0; j < (box_width - 2 - v_skill); j++) printf(" ");
                printf(CYAN "│" RESET "\n");
                
                sprintf(line, "      - %s", s->description);
                int v_desc = get_visual_width(line);
                printf(CYAN "│" RESET WHITE "%s", line);
                for(int j=0; j < (box_width - 2 - v_desc); j++) printf(" "); 
                printf(CYAN "│" RESET "\n");
            }
            if (i < visible_count - 1) {
                printf(CYAN "│" RESET); for(int k=0; k<box_width-2; k++) printf(" "); printf(CYAN "│" RESET "\n");
            }
        }

        printf(CYAN "├"); for(int i=0; i<box_width-2; i++) printf("─"); printf("┤" RESET "\n");
        sprintf(line, "  0. 뒤로 가기");
        int v_back = get_visual_width(line);
        printf(CYAN "│" RESET "%s", line);
        for(int i=0; i < (box_width - 2 - v_back); i++) printf(" ");
        printf(CYAN "│" RESET "\n");
        printf(CYAN "└"); for(int i=0; i<box_width-2; i++) printf("─"); printf("┘" RESET "\n");

        printf("\n  투자할 스킬 번호 선택: ");
        int choice;
        if (scanf("%d", &choice) != 1 || choice == 0) {
            clear_input_buffer();
            break;
        }
        clear_input_buffer();

        if (choice < 1 || choice > visible_count) {
            printf(RED "잘못된 번호입니다.\n" RESET);
            wait_for_enter();
            continue;
        }

        int selected_db_idx = visible_indices[choice - 1];
        Skill* sel = &skill_db[selected_db_idx];

        // 해금 체크 다시 한번
        if (sel->prereq_id != -1) {
            int prereq_lv = 0;
            for (int k = 0; k < p->skill_count; k++) {
                if (p->learned_skills[k].id == sel->prereq_id) {
                    prereq_lv = p->learned_skills[k].level;
                    break;
                }
            }
            if (prereq_lv < 5) {
                printf(RED "선행 스킬 레벨이 부족합니다! (필요: ID %d Lv.5)\n" RESET, sel->prereq_id);
                wait_for_enter();
                continue;
            }
        }

        if (p->skill_points <= 0) {
            printf(RED "스킬 포인트가 부족합니다!\n" RESET);
            wait_for_enter();
            continue;
        }

        // 학습/레벨업 로직
        int learned_idx = -1;
        for (int k = 0; k < p->skill_count; k++) {
            if (p->learned_skills[k].id == sel->id) {
                learned_idx = k;
                break;
            }
        }

        if (learned_idx != -1 && p->learned_skills[learned_idx].level >= sel->max_level) {
            printf(YELLOW "이미 최고 레벨입니다!\n" RESET);
            wait_for_enter();
            continue;
        }

        p->skill_points--;
        if (learned_idx == -1) {
            // 처음 배우는 경우
            p->learned_skills[p->skill_count] = *sel;
            p->learned_skills[p->skill_count].level = 1;
            
            // 초기 스탯 보너스 즉시 반영
            p->max_hp += sel->hp_bonus; p->hp += sel->hp_bonus;
            p->max_mp += sel->mp_bonus; p->mp += sel->mp_bonus;
            p->str += sel->str_bonus;
            p->dex += sel->dex_bonus;
            p->intel += sel->int_bonus;
            p->luk += sel->luk_bonus;
            
            p->skill_count++;
            printf(GREEN "\n✨ [%s] 스킬을 새로 배웠습니다! ✨\n" RESET, sel->name);
        } else {
            // 레벨업
            p->learned_skills[learned_idx].level++;
            
            // 레벨업 시 추가 스탯 반영 (현재 구조상 매 레벨 보너스가 누적됨)
            p->max_hp += sel->hp_bonus; p->hp += sel->hp_bonus;
            p->max_mp += sel->mp_bonus; p->mp += sel->mp_bonus;
            p->str += sel->str_bonus;
            p->dex += sel->dex_bonus;
            p->intel += sel->int_bonus;
            p->luk += sel->luk_bonus;
            
            printf(GREEN "\n🔥 [%s] 스킬이 Lv.%d로 강화되었습니다! 🔥\n" RESET, sel->name, p->learned_skills[learned_idx].level);
        }
        update_combat_power(p);
        wait_for_enter();
    }
}

void select_level_up_skill(Player* p) {
    // 이제 스킬 트리를 통해 직접 올리므로, 단순히 안내만 출력함
    printf("\n[알림] 스킬 포인트를 획득했습니다! '스킬 트리' 메뉴에서 원하는 기술을 연마하세요.\n");
}

void show_skills(Player* p) {
    print_divider(80, MAGENTA);
    print_centered(MAGENTA_BG BLACK " [ 📜 연마된 스킬 리스트 ] " RESET, 80);
    print_divider(80, MAGENTA);
    
    if (p->skill_count == 0) {
        print_box_line("아직 연마한 기술이 없습니다.", 80, MAGENTA);
    } else {
        char buf[256];
        for (int i = 0; i < p->skill_count; i++) {
            Skill* s = &p->learned_skills[i];
            const char* extra = (s->is_active && s->is_passive) ? "[Active+Passive]" : (s->is_active ? "[Active]" : "[Passive]");
            sprintf(buf, "%d. %s (Lv.%d/%d) %s", i + 1, s->name, s->level, s->max_level, extra);
            print_box_line(buf, 80, MAGENTA);
            sprintf(buf, "   ↳ %s", s->description);
            print_box_line(buf, 80, MAGENTA);
        }
    }
    print_divider(80, MAGENTA);
}

void grant_combo_skill_if_eligible(Player* p) {
    if (p->level < 20) return;

    const char* combo_name = NULL;
    
    // 직업별 적합한 콤보 스킬 결정
    switch (p->job) {
        case JOB_WARRIOR: case JOB_GLADIATOR: case JOB_BERSERKER: case JOB_CHAMPION: case JOB_JUDGE:
            combo_name = "[진·용참선]";
            break;
        case JOB_ARCHER: case JOB_RANGER: case JOB_GRANDMAGE:
            combo_name = "[신·천궁살]";
            break;
        case JOB_MAGE: case JOB_SAGE: case JOB_CRUSADER: case JOB_PALADIN:
            combo_name = "[극·명도권]";
            break;
        case JOB_THIEF: case JOB_ASSASSIN: case JOB_AVATAR:
            combo_name = "[비·영격참]";
            break;
        default:
            return;
    }

    if (combo_name == NULL) return;

    // 이미 배웠는지 확인
    for (int i = 0; i < p->skill_count; i++) {
        if (strcmp(p->learned_skills[i].name, combo_name) == 0) return;
    }

    // DB에서 스킬 정보 찾아 증정
    for (int i = 0; i < skill_db_count; i++) {
        if (strcmp(skill_db[i].name, combo_name) == 0) {
            if (p->skill_count < MAX_LEARNED_SKILLS) {
                p->learned_skills[p->skill_count] = skill_db[i];
                p->skill_count++;
                printf("\n" MAGENTA BOLD "🌟 [습득] 레벨 20 달성 기념! 직업 비기 [%s]를 터득했습니다! 🌟" RESET "\n", combo_name);
                printf("   - %s\n", skill_db[i].description);
                wait_for_enter();
            }
            break;
        }
    }
}

