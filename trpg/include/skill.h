#ifndef SKILL_H
#define SKILL_H

#include "job.h"

#define MAX_LEARNED_SKILLS 50
#define MAX_SKILL_LEVEL 10

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

#endif
