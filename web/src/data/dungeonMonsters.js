/**
 * 던전별 출현 몬스터 목록
 *
 * 각 던전 ID에 대해 등장 가능한 몬스터 이름 배열을 정의합니다.
 * 전투 시작 시 배열에서 무작위로 1마리가 선택됩니다.
 *
 * 몬스터 드롭 정보는 monsterLoot.js 를 참고하세요.
 */

/** @type {Record<string, string[]>} */
export const DUNGEON_MONSTERS = {
  field:  ['슬라임', '주황버섯', '스텀프'],
  forest: ['와일드보어', '다크스텀프', '루팡'],
  cave:   ['뿔버섯', '좀비버섯', '이블아이'],
  ruins:  ['해골 군인', '고스트', '사신'],
  abyss:  ['데몬', '가고일', '심연의 정령'],
  dragon: ['와이번', '용의 파수꾼', '레드 드래곤'],
};

/**
 * 던전 ID에 해당하는 몬스터 중 하나를 무작위로 반환합니다.
 * 알 수 없는 던전 ID는 field로 폴백합니다.
 *
 * @param {string} dungeonId
 * @returns {string} 몬스터 이름
 */
export const pickRandomMonster = (dungeonId) => {
  const pool = DUNGEON_MONSTERS[dungeonId] ?? DUNGEON_MONSTERS.field;
  return pool[Math.floor(Math.random() * pool.length)];
};
