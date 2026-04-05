#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/character.h"
#include "../include/utils.h"

#define ALL_SKILLS_COUNT 20

static Skill skill_db[ALL_SKILLS_COUNT];
static int skill_db_count = 0;

void add_to_db(const char* name, const char* desc, float mult, int atk, int luk, SkillType type, JobType job) {
    if (skill_db_count >= ALL_SKILLS_COUNT) return;
    strcpy(skill_db[skill_db_count].name, name);
    strcpy(skill_db[skill_db_count].description, desc);
    skill_db[skill_db_count].level = 1;
    skill_db[skill_db_count].multiplier = mult;
    skill_db[skill_db_count].base_atk_bonus = atk;
    skill_db[skill_db_count].luk_bonus = luk;
    skill_db[skill_db_count].type = type;
    skill_db[skill_db_count].required_job = job;
    skill_db_count++;
}

void init_skill_system() {
    skill_db_count = 0;
    // 공통 스킬
    add_to_db("기초 체력 단련", "영구적으로 공격력이 상승합니다.", 0.0f, 10, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("집중력 강화", "주사위 데미지 배수가 상승합니다.", 0.2f, 0, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("운수 좋은 날", "기본 공격력과 행운(LUK)이 상승합니다.", 0.0f, 3, 10, SKILL_TYPE_COMMON, JOB_NONE);

    // 전사 스킬
    add_to_db("파워 스트라이크", "강력한 일격을 가합니다.", 1.5f, 20, 0, SKILL_TYPE_JOB, JOB_WARRIOR);
    add_to_db("가드 브레이크", "적의 방어를 무시하는 일격입니다.", 1.0f, 15, 0, SKILL_TYPE_JOB, JOB_WARRIOR);
    add_to_db("슬래시 블러스트", "검기를 날려 광역 피해를 입힙니다.", 1.2f, 10, 0, SKILL_TYPE_JOB, JOB_WARRIOR);
    add_to_db("아이언 보디", "방어력을 희생해 공력을 올립니다.", 0.5f, 30, 0, SKILL_TYPE_JOB, JOB_WARRIOR);

    // 궁수 스킬
    add_to_db("에로우 블로우", "빠른 화살 한 발을 발사합니다.", 1.4f, 15, 0, SKILL_TYPE_JOB, JOB_ARCHER);
    add_to_db("더블 샷", "두 발의 화살을 동시에 발사합니다.", 1.8f, 5, 0, SKILL_TYPE_JOB, JOB_ARCHER);
    add_to_db("포커스", "명중률과 공격 배수를 높입니다.", 0.8f, 0, 0, SKILL_TYPE_JOB, JOB_ARCHER);
    add_to_db("크리티컬 샷", "치명적인 화살을 날립니다.", 2.5f, 0, 0, SKILL_TYPE_JOB, JOB_ARCHER);

    // 마법사 스킬
    add_to_db("에너지 볼트", "작은 마력 구체를 날립니다.", 1.2f, 10, 0, SKILL_TYPE_JOB, JOB_MAGE);
    add_to_db("매직 클로", "적을 할퀴는 마력 손톱입니다.", 1.8f, 5, 0, SKILL_TYPE_JOB, JOB_MAGE);
    add_to_db("마나 강화", "최대 MP와 마력이 상승합니다.", 0.5f, 20, 0, SKILL_TYPE_JOB, JOB_MAGE);
    add_to_db("메디테이션", "명상을 통해 공격 배수를 대폭 높입니다.", 1.0f, 0, 0, SKILL_TYPE_JOB, JOB_MAGE);

    // 도적 스킬
    add_to_db("럭키 세븐", "행운을 담은 표창 두 개를 던집니다.", 2.0f, 0, 0, SKILL_TYPE_JOB, JOB_THIEF);
    add_to_db("디스오더", "적의 방어력을 깎고 공격 배수를 높입니다.", 0.7f, 10, 0, SKILL_TYPE_JOB, JOB_THIEF);
    add_to_db("헤이스트", "민첩하게 움직여 추가 피해를 줍니다.", 0.4f, 15, 0, SKILL_TYPE_JOB, JOB_THIEF);
    add_to_db("세비지 블로우", "적을 난도질하여 큰 피해를 입힙니다.", 3.0f, -10, 0, SKILL_TYPE_JOB, JOB_THIEF);

    // 하이브리드/전설 스킬들은 확장이 용이하도록 설계됨... (생략)
}

const char* get_skill_type_name(SkillType type) {
    switch (type) {
        case SKILL_TYPE_COMMON: return "공통";
        case SKILL_TYPE_JOB: return "직업";
        case SKILL_TYPE_ULTIMATE: return "궁극기";
        default: return "알 수 없음";
    }
}

void apply_skill_effects(Player* p) {
    // 이 함수는 전투 시작 전이나 스탯 갱신 시 호출됨
    // 실제 데미지 계산 로직(combat.c)에서 스킬 목록을 순회하여 사용함
}

void select_level_up_skill(Player* p) {
    if (skill_db_count == 0) init_skill_system();
    
    // 특정 레벨(예: 30)에 궁극기 오픈 로직 추가 가능
    if (p->level >= 30 && !p->has_ultimate) {
        printf("\n🌟 [각성] 직업의 정점에 도달하여 궁극기를 사용할 수 있게 되었습니다! 🌟\n");
        p->has_ultimate = 1;
        // 궁극기 설정 (직업별로 다르게)
        strcpy(p->ultimate_skill.name, "전설의 심판");
        strcpy(p->ultimate_skill.description, "모든 힘을 쏟아부어 세상을 정화합니다.");
        p->ultimate_skill.multiplier = 10.0f;
        p->ultimate_skill.level = 1;
        p->ultimate_skill.type = SKILL_TYPE_ULTIMATE;
        wait_for_enter();
    }

    int option_indices[3];
    int found_count = 0;

    // 1. 후보군 생성
    // - 공통 스킬
    // - 자신의 직업 스킬
    // - 이미 배운 스킬 중 레벨 5 미만인 것

    int pool_indices[ALL_SKILLS_COUNT + MAX_LEARNED_SKILLS];
    int pool_count = 0;

    // DB에서 가능한 스킬 찾기
    for (int i = 0; i < skill_db_count; i++) {
        if (skill_db[i].type == SKILL_TYPE_COMMON || skill_db[i].required_job == p->job) {
            // 이미 배운 스킬인지 확인
            int already_learned = -1;
            for (int k = 0; k < p->skill_count; k++) {
                if (strcmp(p->learned_skills[k].name, skill_db[i].name) == 0) {
                    already_learned = k;
                    break;
                }
            }

            if (already_learned == -1) {
                if (p->skill_count < MAX_LEARNED_SKILLS) {
                    pool_indices[pool_count++] = i; // 새로 배울 수 있는 스킬
                }
            } else {
                if (p->learned_skills[already_learned].level < MAX_SKILL_LEVEL) {
                    pool_indices[pool_count++] = i; // 레벨업 가능한 기존 스킬
                }
            }
        }
    }

    if (pool_count == 0) {
        printf("\n[알림] 더 이상 배울 수 있는 스킬이 없습니다.\n");
        wait_for_enter();
        return;
    }

    // 2. 무작위 3개 선택
    for (int i = 0; i < 3 && pool_count > 0; i++) {
        int r = rand() % pool_count;
        int db_idx = pool_indices[r];
        option_indices[found_count] = db_idx;
        
        // 중복 방지를 위해 pool에서 제거
        pool_indices[r] = pool_indices[pool_count - 1];
        pool_count--;
        found_count++;
    }

    // 3. UI 출력
    clear_screen();
    printf("\n========= ✨ 스킬 선택 ✨ =========\n");
    printf("레벨업 기념으로 새로운 힘을 얻을 수 있습니다!\n\n");

    for (int i = 0; i < found_count; i++) {
        int db_idx = option_indices[i];
        Skill* s = &skill_db[db_idx];
        
        int learned_idx = -1;
        for (int k = 0; k < p->skill_count; k++) {
            if (strcmp(p->learned_skills[k].name, s->name) == 0) {
                learned_idx = k;
                break;
            }
        }

        if (learned_idx == -1) {
            printf("%d. [NEW] %s (Lv.1)\n", i + 1, s->name);
        } else {
            printf("%d. [UP] %s (Lv.%d -> %d)\n", i + 1, s->name, 
                   p->learned_skills[learned_idx].level, p->learned_skills[learned_idx].level + 1);
        }
        printf("   - %s\n", s->description);
        if (s->multiplier > 0) printf("   - 데미지 배수: +%.1f\n", s->multiplier);
        if (s->base_atk_bonus > 0) printf("   - 기본 공격력: +%d\n", s->base_atk_bonus);
        if (s->luk_bonus > 0) printf("   - 행운(LUK) : +%d\n", s->luk_bonus);
        printf("\n");
    }

    printf("선택: ");
    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > found_count) {
        clear_input_buffer();
        printf("올바른 번호를 선택하세요. 첫 번째 스킬로 자동 선택됩니다.\n");
        choice = 1;
    }
    clear_input_buffer();

    int selected_db_idx = option_indices[choice - 1];
    Skill* sel = &skill_db[selected_db_idx];

    // 4. 적용
    int learned_idx = -1;
    for (int k = 0; k < p->skill_count; k++) {
        if (strcmp(p->learned_skills[k].name, sel->name) == 0) {
            learned_idx = k;
            break;
        }
    }

    if (learned_idx == -1) {
        // 새로 배우기
        p->learned_skills[p->skill_count] = *sel;
        p->learned_skills[p->skill_count].level = 1;
        p->luk += sel->luk_bonus; // 스탯 즉시 반영
        printf("\n✨ [%s] 스킬을 새로 배웠습니다! ✨\n", sel->name);
        p->skill_count++;
    } else {
        // 레벨업
        p->learned_skills[learned_idx].level++;
        p->luk += sel->luk_bonus; // 스탯 즉시 반영
        printf("\n🔥 [%s] 스킬이 LV.%d로 강화되었습니다! 🔥\n", sel->name, p->learned_skills[learned_idx].level);
    }

    wait_for_enter();
}

void show_skills(Player* p) {
    printf("\n========= [보유 스킬] (%d / %d) =========\n", p->skill_count, MAX_LEARNED_SKILLS);
    if (p->skill_count == 0) {
        printf("배운 스킬이 없습니다.\n");
    } else {
        for (int i = 0; i < p->skill_count; i++) {
            Skill* s = &p->learned_skills[i];
            printf("%d. %s (Lv.%d) [%s]\n", i + 1, s->name, s->level, get_skill_type_name(s->type));
            printf("   - %s\n", s->description);
        }
    }
    if (p->has_ultimate) {
        printf("🌟 궁극기: %s (Lv.%d)\n", p->ultimate_skill.name, p->ultimate_skill.level);
    }
    printf("========================================\n");
}
