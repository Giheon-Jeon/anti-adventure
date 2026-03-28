#ifndef CHARACTER_H
#define CHARACTER_H

typedef struct {
    char name[50];
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
} Player;

// 플레이어 초기화 함수
void init_player(Player* p);

// 플레이어 상태 출력 함수
void show_status(const Player* p);
void show_compact_status(const Player* p);

// 레벨업 체크 및 처리 함수
void check_level_up(Player* p);

// 사망 패널티 적용 함수
void apply_death_penalty(Player* p);

#endif
