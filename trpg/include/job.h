#ifndef JOB_H
#define JOB_H

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

#endif
