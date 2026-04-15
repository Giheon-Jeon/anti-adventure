#ifndef ABILITY_H
#define ABILITY_H

#include "character.h"

// 어빌리티 메뉴 진입
void open_ability_menu(Player* p);

// 어빌리티 등급 이름 반환 (NORMAL, RARE, EPIC 등)
const char* get_ability_rank_name(AbilityRank r);

// 어빌리티 타입 이름 반환 (STR 증가 등)
const char* get_ability_type_name(AbilityType t);

// 랜덤 어빌리티 생성
Ability generate_random_ability();

#endif
