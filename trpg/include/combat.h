#ifndef COMBAT_H
#define COMBAT_H

#include "character.h"
#include "item.h"

/**
 * @struct Monster
 * @brief 몬스터의 정보를 나타내는 구조체
 */
typedef struct {
    int id;             /**< 고유 ID */
    char name[50];      /**< 몬스터 이름 */
    int hp;             /**< 현재 체력 */
    int max_hp;         /**< 최대 체력 */
    int def;            /**< 방어력 */
    int is_boss;        /**< 보스 여부 (0: 일반, 1: 보스) */
    Item loot;          /**< 전리품 정보 */
} Monster;

/**
 * @struct Dungeon
 * @brief 던전 정보와 등장 몬스터를 관리하는 구조체
 */
typedef struct {
    char name[50];      /**< 던전 이름 */
    int min_cp;         /**< 최소 요구 전투력 */
    int max_cp;         /**< 최대 제한 전투력 */
    Monster monsters[3]; /**< 일반 몬스터 목록 */
    int monster_count;   /**< 몬스터 수 */
    Monster boss;       /**< 보스 몬스터 */
    int has_boss;       /**< 보스 존재 여부 */
} Dungeon;

/**
 * @brief 주사위를 굴립니다.
 * @param dice 결과를 저장할 배열
 * @param count 굴릴 개수
 */
void roll_dice(int* dice, int count);

/**
 * @brief 주사위를 정렬합니다.
 */
void sort_dice(int* dice, int count);

/**
 * @brief 야추(Yacht) 데미지를 계산합니다.
 */
int calculate_yacht_damage(Player* p, int dice[5]);

/**
 * @brief 최종 가한 데미지를 계산합니다.
 */
int calculate_final_damage(Player* p, Monster* m, int yacht_result);

/**
 * @brief 플레이어가 받는 데미지 산출 (방어구 내구도 반영)
 */
int calculate_player_taken_damage(Player* p, int raw_enemy_atk);

/**
 * @brief 전투를 시작합니다.
 */
void start_combat(Player *p, Dungeon *d);

/**
 * @brief 던전을 선택합니다.
 */
void select_dungeon(Player* p);

#endif
