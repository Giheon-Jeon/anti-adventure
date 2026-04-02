#ifndef ABILITY_H
#define ABILITY_H

#include "character.h"

// 어빌리티 메뉴 진입
void open_ability_menu(Player* p);

// 어빌리티 이름 반환 (예: STR +20% / STR +30)
const char* get_ability_name(Ability a);

// 어빌리티 수치 문자열 반환
const char* get_ability_rank_name(AbilityRank r);

#endif
