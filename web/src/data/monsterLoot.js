/**
 * 몬스터 드롭 아이템 테이블
 *
 * 각 몬스터마다 드롭 아이템 이름(name)과 드롭 확률(rate)을 정의합니다.
 * 드롭된 아이템은 인벤토리에 쌓이며, 대장간 제작 재료로 사용됩니다.
 *
 * 드롭 확률 기준:
 * - 초반(field): 0.6  (60%)
 * - 중반(forest, cave, ruins): 0.4~0.5
 * - 후반(abyss, dragon): 0.2~0.4  (희귀 재료)
 */

/** @type {Record<string, { name: string, rate: number }>} */
export const MONSTER_LOOT_MAP = {
  // 초심자의 들판 (field)
  '슬라임':    { name: '슬라임 방울', rate: 0.6 },
  '주황버섯':  { name: '버섯 갓',    rate: 0.6 },
  '스텀프':    { name: '나뭇가지',   rate: 0.6 },

  // 어두운 숲 (forest)
  '와일드보어':  { name: '와일드보어의 송곳니', rate: 0.5 },
  '다크스텀프':  { name: '검은 나뭇가지',       rate: 0.5 },
  '루팡':        { name: '루팡의 바나나',        rate: 0.5 },

  // 고블린 동굴 (cave)
  '뿔버섯':    { name: '뿔버섯의 갓',   rate: 0.4 },
  '좀비버섯':  { name: '부적',          rate: 0.4 },
  '이블아이':  { name: '이블아이의 꼬리', rate: 0.4 },

  // 저주받은 폐허 (ruins)
  '해골 군인': { name: '해골 조각',   rate: 0.4 },
  '고스트':    { name: '유령의 눈물', rate: 0.4 },
  '사신':      { name: '해골 조각',   rate: 0.5 },

  // 심연의 나락 (abyss)
  '데몬':        { name: '심연의 정수', rate: 0.3 },
  '가고일':      { name: '마계의 돌',   rate: 0.3 },
  '심연의 정령': { name: '심연의 정수', rate: 0.4 },

  // 드래곤의 둥지 (dragon)
  '와이번':      { name: '용의 비늘',    rate: 0.3 },
  '용의 파수꾼': { name: '용의 비늘',    rate: 0.4 },
  '레드 드래곤': { name: '드래곤의 심장', rate: 0.2 }, // 제일 희귀한 재료
};

/**
 * 몬스터 이름을 받아 드롭 아이템 배열을 반환합니다.
 * 드롭 테이블에 없거나 확률 실패 시 빈 배열을 반환합니다.
 *
 * @param {string} monsterName
 * @returns {Array<{ name: string, count: number }>}
 */
export const getMonsterDrops = (monsterName) => {
  const loot = MONSTER_LOOT_MAP[monsterName];
  if (loot && Math.random() < loot.rate) {
    return [{ name: loot.name, count: 1 }];
  }
  return [];
};
