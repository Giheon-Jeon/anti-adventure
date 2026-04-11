#ifndef CHARACTER_H
#define CHARACTER_H

#include "item.h"
#include <time.h>

#include "job.h"

#define MAX_LEARNED_SKILLS 50
#define MAX_SKILL_LEVEL 10
#define MAX_BOOK_ENTRIES 20

typedef enum {
    SKILL_TYPE_COMMON,
    SKILL_TYPE_JOB,
    SKILL_TYPE_ULTIMATE,
    SKILL_TYPE_COMBO
} SkillType;

typedef struct {
    int id;             // 고유 ID
    char name[50];
    char description[100];
    int level;          // 현재 스킬 레벨 (투자된 포인트)
    int max_level;      // 최대 레벨 (기본 10)
    
    float multiplier;
    int base_atk_bonus;
    int luk_bonus;
    int hp_bonus;
    int mp_bonus;
    int str_bonus;
    int dex_bonus;
    int int_bonus;
    int mp_cost;
    
    SkillType type;
    JobType required_job;
    
    int tier;           // 계층 (0:공통, 1:기본, 2:대가)
    int prereq_id;      // 선행 스킬 ID (-1이면 없음)
    int is_passive;     // 패시브 효과 존재 여부
    int is_active;      // 액티브 효과 존재 여부
} Skill;

#define JOB_ADVANCEMENT_LEVEL 10

typedef enum {
    ABILITY_RANK_NORMAL = 0,
    ABILITY_RANK_RARE,
    ABILITY_RANK_EPIC,
    ABILITY_RANK_UNIQUE,
    ABILITY_RANK_LEGENDARY
} AbilityRank;

typedef enum {
    ABILITY_TYPE_NONE = 0,
    ABILITY_TYPE_STR_FLAT,
    ABILITY_TYPE_DEX_FLAT,
    ABILITY_TYPE_INT_FLAT,
    ABILITY_TYPE_LUK_FLAT,
    ABILITY_TYPE_STR_PER,
    ABILITY_TYPE_DEX_PER,
    ABILITY_TYPE_INT_PER,
    ABILITY_TYPE_LUK_PER,
    ABILITY_TYPE_BOSS_DMG,
    ABILITY_TYPE_IED,
    ABILITY_TYPE_DMG_PER,
    ABILITY_TYPE_DOUBLE_ATTACK
} AbilityType;

typedef struct {
    AbilityRank rank;
    AbilityType type;
    float value;
} Ability;

typedef struct {
    int id;             // 몬스터 ID
    int is_registered;  // 등록 여부
    int kill_count;     // 누적 처치 수
} MonsterBookEntry;

#define ABILITY_COUNT 3

typedef struct {
    char name[50];
    JobType job;       // 캐릭터 직업
    int level;
    int hp;
    int max_hp;
    // 4대 주요 능력치
    int str;
    int dex;
    int intel; // int는 예약어이므로 intel로 명명
    int luk;
    
    // 전투 상세 스탯
    int mp;
    int max_mp;
    int magic_atk;     // 마력
    float ied;         // 방어율 무시 (0.0 ~ 1.0)
    float boss_dmg;    // 보스 공격 데미지 (0.0 ~ 1.0)
    float dmg_percent; // 데미지 % 상승 (0.0 ~ 1.0)
    
    int exp;
    int gold;
    int stat_points;   // 레벨업 시 얻는 스탯 포인트
    int skill_points;  // 레벨업 시 얻는 스킬 포인트
    
    // 장비 단계 관리 (상점 구매 - 기본 스탯 중심)
    int weapon_tier;
    int armor_tier;
    int accessory_tier;

    // 제작 장비 단계 관리 (대장간 제작 - 특수 스탯 중심)
    int c_weapon_tier;
    int c_armor_tier;
    int c_accessory_tier;
    
    // 장비 내구도 관리 (0 ~ 100)
    int weapon_dur;
    int armor_dur;
    int accessory_dur;
    int c_weapon_dur;
    int c_armor_dur;
    int c_accessory_dur;
    
    int combat_power;   // 종합 전투력
    
    // 알바 관련 데이터
    time_t last_job_time;
    int job_count;
    
    Ability abilities[ABILITY_COUNT];
    int ability_locked[ABILITY_COUNT]; // 0: unlock, 1: lock
    
    Inventory inventory;
    
    Skill learned_skills[MAX_LEARNED_SKILLS];
    int skill_count;
    Skill ultimate_skill;
    int has_ultimate;

    MonsterBookEntry encyclopedia[MAX_BOOK_ENTRIES];
} Player;

// 플레이어 초기화 함수
void init_player(Player* p);

// 플레이어 상태 출력 함수
void show_status(Player* p);
void show_compact_status(Player* p);
void show_inventory(const Player* p);

// 레벨업 체크 및 처리 함수
void check_level_up(Player* p);

// 스킬 시스템 관련 함수
void select_level_up_skill(Player* p);
void grant_event_skill(Player* p, const char* name, const char* desc, float mult, int atk, int luk, int hp, int mp, int str, int dex, int intel, int cost);
void show_skills(Player* p);
void show_skill_tree(Player* p); // 새 스킬 트리 UI
void init_skill_system();
const char* get_skill_type_name(SkillType type);
void apply_skill_effects(Player* p);
void grant_combo_skill_if_eligible(Player* p);

// 사망 패널티 적용 함수
void apply_death_penalty(Player* p);

// 전투력 업데이트 함수
void update_combat_power(Player* p);

// 도감 관련 함수
void show_monster_encyclopedia(Player* p);

#endif
