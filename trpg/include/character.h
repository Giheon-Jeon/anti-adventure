#ifndef CHARACTER_H
#define CHARACTER_H

#include "item.h"
#include <time.h>

#include "job.h"

#define MAX_LEARNED_SKILLS 10
#define MAX_SKILL_LEVEL 5

typedef enum {
    SKILL_TYPE_COMMON,
    SKILL_TYPE_JOB,
    SKILL_TYPE_ULTIMATE
} SkillType;

typedef struct {
    char name[50];
    char description[100];
    int level;
    float multiplier;
    int base_atk_bonus;
    int luk_bonus;
    int hp_bonus;       // 추가: 최대 HP 보너스
    int mp_bonus;       // 추가: 최대 MP 보너스
    int str_bonus;      // 추가: 힘 보너스
    int dex_bonus;      // 추가: 민첩 보너스
    int int_bonus;      // 추가: 지력 보너스
    int mp_cost;        // 추가: 스킬 발동 시 MP 소모 (패시브는 0)
    SkillType type;
    JobType required_job;
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
    int stat_points;   // 레벨업 시 얻는 포인트
    
    // 장비 단계 관리 (상점 구매 - 기본 스탯 중심)
    int weapon_tier;
    int armor_tier;
    int accessory_tier;

    // 제작 장비 단계 관리 (대장간 제작 - 특수 스탯 중심)
    int c_weapon_tier;
    int c_armor_tier;
    int c_accessory_tier;
    
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
void show_skills(Player* p);
void init_skill_system();
const char* get_skill_type_name(SkillType type);
void apply_skill_effects(Player* p);

// 사망 패널티 적용 함수
void apply_death_penalty(Player* p);

// 전투력 업데이트 함수
void update_combat_power(Player* p);

#endif
