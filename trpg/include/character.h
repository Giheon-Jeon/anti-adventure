#ifndef CHARACTER_H
#define CHARACTER_H

#include "item.h"
#include <time.h>

typedef enum {
    JOB_NONE = 0,
    // 기본 직업
    JOB_WARRIOR,    // STR
    JOB_ARCHER,     // DEX
    JOB_MAGE,       // INT
    JOB_THIEF,      // LUK
    
    // 2개 동률 (Hybrid)
    JOB_GLADIATOR,  // STR+DEX
    JOB_CRUSADER,   // STR+INT
    JOB_BERSERKER,  // STR+LUK
    JOB_RANGER,     // DEX+INT
    JOB_ASSASSIN,   // DEX+LUK
    JOB_SAGE,       // INT+LUK
    
    // 3개 동률 (Elite)
    JOB_CHAMPION,   // STR+DEX+INT
    JOB_JUDGE,      // STR+DEX+LUK
    JOB_PALADIN,    // STR+INT+LUK
    JOB_GRANDMAGE,  // DEX+INT+LUK
    
    // 4개 동률 (Legendary)
    JOB_AVATAR      // STR+DEX+INT+LUK
} JobType;

#define JOB_ADVANCEMENT_LEVEL 10

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
    
    // 장비/스킬 단계 관리
    int weapon_tier;
    int armor_tier;
    int accessory_tier;
    
    int combat_power;   // 종합 전투력
    
    // 알바 관련 데이터
    time_t last_job_time;
    int job_count;
    
    Inventory inventory;
} Player;

// 플레이어 초기화 함수
void init_player(Player* p);

// 플레이어 상태 출력 함수
void show_status(Player* p);
void show_compact_status(Player* p);
void show_inventory(const Player* p);

// 레벨업 체크 및 처리 함수
void check_level_up(Player* p);

// 사망 패널티 적용 함수
void apply_death_penalty(Player* p);

// 전투력 업데이트 함수
void update_combat_power(Player* p);

#endif
