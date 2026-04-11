#ifndef JOB_H
#define JOB_H

typedef enum {
    JOB_NONE = 0,
    // 기본 직업
    JOB_WARRIOR,    // STR
    JOB_ARCHER,     // DEX
    JOB_MAGE,       // INT
    JOB_THIEF,      // LUK
    
    // 2차 전직 (Hybrid)
    JOB_GLADIATOR,  // STR+DEX
    JOB_CRUSADER,   // STR+INT
    JOB_BERSERKER,  // STR+LUK
    JOB_RANGER,     // DEX+INT
    JOB_ASSASSIN,   // DEX+LUK
    JOB_SAGE,       // INT+LUK
    
    // 3차 전직 (Elite)
    JOB_CHAMPION,   // STR+DEX+INT
    JOB_JUDGE,      // STR+DEX+LUK
    JOB_PALADIN,    // STR+INT+LUK
    JOB_GRANDMAGE,  // DEX+INT+LUK
    
    // 4차 전직 (Legendary)
    JOB_AVATAR      // STR+DEX+INT+LUK
} JobType;

// 직업 이름을 문자열로 반환하는 함수
const char* get_job_name(JobType job);

#endif
