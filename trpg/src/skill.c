#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/character.h"
#include "../include/utils.h"

#define ALL_SKILLS_COUNT 100

static Skill skill_db[ALL_SKILLS_COUNT];
static int skill_db_count = 0;

void add_to_db(const char* name, const char* desc, float mult, int atk, int luk, int hp, int mp, int str, int dex, int intel, int cost, SkillType type, JobType job) {
    if (skill_db_count >= ALL_SKILLS_COUNT) return;
    Skill* s = &skill_db[skill_db_count];
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
    s->type = type;
    s->required_job = job;
    skill_db_count++;
}

void init_skill_system() {
    skill_db_count = 0;
    // --- 공통 스킬 (COMMON) --- (13인자: 이름, 설명, 배수, 공증, 행운, HP, MP, STR, DEX, INT, 소모, 타입, 직업)
    add_to_db("기초 체력 단련", "영구적으로 공격력이 상승합니다.", 0.0f, 10, 0, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("집중력 강화", "주사위 데미지 배수가 상승합니다.", 0.2f, 0, 0, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("운수 좋은 날", "공격력과 행운(LUK)이 함께 상승합니다.", 0.0f, 3, 10, 0, 0, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("강인한 신체", "최대 HP가 대폭 상승합니다.", 0.0f, 0, 0, 150, 0, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("끝없는 마력", "최대 MP가 대폭 상승합니다.", 0.0f, 0, 0, 0, 80, 0, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("검의 달인", "힘(STR)이 영구적으로 상승합니다.", 0.0f, 0, 0, 0, 0, 12, 0, 0, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("명사자의 눈", "민첩(DEX)이 영구적으로 상승합니다.", 0.0f, 0, 0, 0, 0, 0, 12, 0, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("지혜의 수호", "지능(INT)이 영구적으로 상승합니다.", 0.0f, 0, 0, 0, 0, 0, 0, 12, 0, SKILL_TYPE_COMMON, JOB_NONE);
    add_to_db("신속한 몸놀림", "데미지 배수와 민첩이 약간 상승합니다.", 0.1f, 0, 0, 0, 0, 0, 5, 0, 0, SKILL_TYPE_COMMON, JOB_NONE);

    // --- 전사 계열 (Warrior & Hybrids) ---
    add_to_db("파워 스트라이크", "강력한 일격을 가합니다.", 1.5f, 20, 0, 0, 0, 0, 0, 0, 10, SKILL_TYPE_JOB, JOB_WARRIOR);
    add_to_db("가드 브레이크", "방어 무시 데미지를 입힙니다.", 1.2f, 15, 0, 0, 0, 5, 0, 0, 15, SKILL_TYPE_JOB, JOB_WARRIOR);
    add_to_db("슬래시 블러스트", "검기를 날려 광역 피해를 입힙니다.", 1.3f, 10, 0, 0, 0, 0, 0, 0, 12, SKILL_TYPE_JOB, JOB_WARRIOR);
    add_to_db("소드 댄스", "검투사의 화려한 연속 베기입니다.", 2.2f, 30, 0, 0, 0, 10, 10, 0, 25, SKILL_TYPE_JOB, JOB_GLADIATOR);
    add_to_db("홀리 실드", "신성한 힘으로 자신을 보호하며 반격합니다.", 1.8f, 10, 0, 100, 50, 5, 0, 15, 30, SKILL_TYPE_JOB, JOB_CRUSADER);
    add_to_db("피의 갈증", "자신의 HP를 대가로 파괴적인 힘을 얻습니다.", 4.0f, 100, 20, -50, 0, 30, 0, 0, 0, SKILL_TYPE_JOB, JOB_BERSERKER);

    // --- 궁수 계열 (Archer & Hybrids) ---
    add_to_db("에로우 블로우", "빠른 화살 한 발을 발사합니다.", 1.4f, 15, 0, 0, 0, 0, 5, 0, 8, SKILL_TYPE_JOB, JOB_ARCHER);
    add_to_db("더블 샷", "두 발의 화살을 동시에 발사합니다.", 1.9f, 5, 0, 0, 0, 0, 8, 0, 12, SKILL_TYPE_JOB, JOB_ARCHER);
    add_to_db("원드 애로우", "바람의 힘을 실은 화살입니다.", 2.5f, 20, 0, 0, 30, 0, 15, 15, 20, SKILL_TYPE_JOB, JOB_RANGER);
    add_to_db("스나이핑", "적의 급소를 정확히 꿰뚫습니다.", 5.5f, 50, 0, 0, 0, 0, 40, 0, 40, SKILL_TYPE_JOB, JOB_RANGER);
    add_to_db("트리플 샷", "세 발의 화살을 전방으로 발사합니다.", 3.0f, 15, 0, 0, 0, 0, 20, 0, 25, SKILL_TYPE_JOB, JOB_RANGER);

    // --- 마법사 계열 (Mage & Hybrids) ---
    add_to_db("에너지 볼트", "작은 마력 구체를 날립니다.", 1.2f, 10, 0, 0, 10, 0, 0, 5, 10, SKILL_TYPE_JOB, JOB_MAGE);
    add_to_db("매직 클로", "적을 할퀴는 마력 손톱입니다.", 2.0f, 5, 0, 0, 0, 0, 0, 8, 15, SKILL_TYPE_JOB, JOB_MAGE);
    add_to_db("지혜의 샘", "지능과 마나를 대폭 상승시킵니다.", 0.5f, 0, 0, 0, 150, 0, 0, 30, 50, SKILL_TYPE_JOB, JOB_SAGE);
    add_to_db("이클립스", "태양을 가리는 거대한 마법 폭발입니다.", 6.0f, 100, 0, 0, 0, 0, 0, 60, 100, SKILL_TYPE_JOB, JOB_GRANDMAGE);
    add_to_db("원소의 폭주", "모든 원소를 융합하여 적을 파괴합니다.", 8.5f, 200, 0, 0, 200, 0, 0, 100, 150, SKILL_TYPE_JOB, JOB_GRANDMAGE);

    // --- 도적 계열 (Thief & Hybrids) ---
    add_to_db("럭키 세븐", "행운을 담은 표창 두 개를 던집니다.", 2.2f, 0, 10, 0, 0, 0, 0, 0, 12, SKILL_TYPE_JOB, JOB_THIEF);
    add_to_db("디스오더", "적의 방어력을 깎고 혼란에 빠뜨립니다.", 1.2f, 10, 5, 0, 0, 0, 5, 0, 15, SKILL_TYPE_JOB, JOB_THIEF);
    add_to_db("맹독 가르기", "독을 바른 단검으로 적을 벱니다.", 2.8f, 40, 15, 0, 0, 0, 15, 0, 25, SKILL_TYPE_JOB, JOB_ASSASSIN);
    add_to_db("그림자 습격", "순식간에 뒤로 이동하여 치명상을 입힙니다.", 4.5f, 60, 30, 0, 0, 0, 25, 0, 35, SKILL_TYPE_JOB, JOB_ASSASSIN);
    add_to_db("다크 플레어", "어둠의 불꽃으로 적을 태웁니다.", 3.5f, 30, 10, 0, 50, 0, 0, 20, 30, SKILL_TYPE_JOB, JOB_ASSASSIN);

    // --- 전설 및 아바타 계열 (Legendary) ---
    add_to_db("진정한 힘", "한계를 돌파한 초인의 힘입니다.", 5.0f, 100, 0, 300, 100, 50, 50, 50, 0, SKILL_TYPE_JOB, JOB_CHAMPION);
    add_to_db("최후의 심판", "심판자의 공정한 처단이 시작됩니다.", 7.7f, 150, 50, 0, 0, 60, 60, 0, 100, SKILL_TYPE_JOB, JOB_JUDGE);
    add_to_db("성궤의 기적", "성궤사에게 내려진 마지막 구원입니다.", 0.2f, 50, 100, 1000, 500, 0, 0, 100, 0, SKILL_TYPE_JOB, JOB_PALADIN);
    add_to_db("천상의 숨결", "신의 가호가 온몸을 감쌉니다.", 10.0f, 300, 100, 500, 500, 80, 80, 80, 200, SKILL_TYPE_JOB, JOB_AVATAR);
    add_to_db("신의 권능", "세상의 법칙조차 거스르는 힘입니다.", 15.0f, 500, 200, 1000, 1000, 100, 100, 100, 300, SKILL_TYPE_JOB, JOB_AVATAR);
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
        if (s->hp_bonus != 0) printf("   - 최대 HP : %s%d\n", (s->hp_bonus > 0) ? "+" : "", s->hp_bonus);
        if (s->mp_bonus != 0) printf("   - 최대 MP : %s%d\n", (s->mp_bonus > 0) ? "+" : "", s->mp_bonus);
        if (s->str_bonus != 0) printf("   - 힘(STR) : %s%d\n", (s->str_bonus > 0) ? "+" : "", s->str_bonus);
        if (s->dex_bonus != 0) printf("   - 민첩(DEX) : %s%d\n", (s->dex_bonus > 0) ? "+" : "", s->dex_bonus);
        if (s->int_bonus != 0) printf("   - 지능(INT) : %s%d\n", (s->int_bonus > 0) ? "+" : "", s->int_bonus);
        if (s->luk_bonus != 0) printf("   - 행운(LUK) : %s%d\n", (s->luk_bonus > 0) ? "+" : "", s->luk_bonus);
        if (s->mp_cost > 0) printf("   " RED "[소모 MP: %d]" RESET "\n", s->mp_cost);
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
        
        // 스탯 즉시 반영
        p->max_hp += sel->hp_bonus; p->hp += sel->hp_bonus;
        p->max_mp += sel->mp_bonus; p->mp += sel->mp_bonus;
        p->str += sel->str_bonus;
        p->dex += sel->dex_bonus;
        p->intel += sel->int_bonus;
        p->luk += sel->luk_bonus;
        
        printf("\n✨ [%s] 스킬을 새로 배웠습니다! ✨\n", sel->name);
        p->skill_count++;
    } else {
        // 레벨업
        p->learned_skills[learned_idx].level++;
        
        // 스탯 보너스 누적 반영
        p->max_hp += sel->hp_bonus; p->hp += sel->hp_bonus;
        p->max_mp += sel->mp_bonus; p->mp += sel->mp_bonus;
        p->str += sel->str_bonus;
        p->dex += sel->dex_bonus;
        p->intel += sel->int_bonus;
        p->luk += sel->luk_bonus;
        
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
