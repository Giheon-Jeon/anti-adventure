/**
 * 대장간 제작 레시피 목록
 *
 * 모든 레시피는 tier 4 전설급 아이템입니다.
 * 제작 조건:
 * 1. 해당 type의 현재 tier가 craftTier - 1 이어야 합니다.
 * 2. 골드(cost)와 재료(materials) 모두 보유해야 합니다.
 *
 * 재료는 던전 몬스터를 처치하여 획득합니다 (monsterLoot.js 참고).
 */

/** @type {Array<{ id: string, name: string, desc: string, type: string, tier: number, cost: number, stat: Object, materials: Array<{ name: string, count: number }> }>} */
export const CRAFT_RECIPES = [
  // ── 전설 무기 ──────────────────────────────────────────────────────────
  {
    id: 'c_w4_1',
    name: '용살자의 대검',
    desc: 'STR+300, DEX+100 (전설 무기)',
    type: 'weapon',
    tier: 4,
    cost: 5000,
    stat: { str: 300, dex: 100 },
    materials: [
      { name: '용의 비늘',    count: 5  },
      { name: '드래곤의 심장', count: 1  },
      { name: '나뭇가지',     count: 10 },
    ],
  },
  {
    id: 'c_w4_2',
    name: '사신의 낫',
    desc: 'STR+200, LUK+150 (전설 무기)',
    type: 'weapon',
    tier: 4,
    cost: 4000,
    stat: { str: 200, luk: 150 },
    materials: [
      { name: '해골 조각',   count: 10 },
      { name: '유령의 눈물', count: 5  },
      { name: '심연의 정수', count: 3  },
    ],
  },

  // ── 전설 방어구 ──────────────────────────────────────────────────────────
  {
    id: 'c_a4_1',
    name: '드래곤 스케일 아머',
    desc: 'HP+10000, MP+3000 (전설 방어구)',
    type: 'armor',
    tier: 4,
    cost: 5000,
    stat: { maxHp: 10000, maxMp: 3000 },
    materials: [
      { name: '용의 비늘',           count: 8  },
      { name: '이블아이의 꼬리',     count: 10 },
      { name: '와일드보어의 송곳니', count: 10 },
    ],
  },
  {
    id: 'c_a4_2',
    name: '심연의 마법 로브',
    desc: 'HP+6000, MP+6000 (전설 방어구)',
    type: 'armor',
    tier: 4,
    cost: 4500,
    stat: { maxHp: 6000, maxMp: 6000 },
    materials: [
      { name: '심연의 정수', count: 5  },
      { name: '마계의 돌',   count: 5  },
      { name: '부적',        count: 10 },
    ],
  },

  // ── 고대 장신구 ──────────────────────────────────────────────────────────
  {
    id: 'c_ac3_1',
    name: '여명의 귀걸이',
    desc: 'AllStat+50 (고대 장신구)',
    type: 'accessory',
    tier: 3,
    cost: 3000,
    stat: { str: 50, dex: 50, int: 50, luk: 50 },
    materials: [
      { name: '유령의 눈물', count: 8  },
      { name: '루팡의 바나나', count: 15 },
      { name: '슬라임 방울', count: 20 },
    ],
  },
  {
    id: 'c_ac3_2',
    name: '드림 브레이커 링',
    desc: 'AllStat+80 (고대 장신구)',
    type: 'accessory',
    tier: 3,
    cost: 6000,
    stat: { str: 80, dex: 80, int: 80, luk: 80 },
    materials: [
      { name: '드래곤의 심장', count: 2 },
      { name: '심연의 정수',   count: 5 },
      { name: '마계의 돌',     count: 8 },
    ],
  },
];
