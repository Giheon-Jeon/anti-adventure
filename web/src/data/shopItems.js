/**
 * 마을 상점 판매 아이템 목록
 *
 * 각 아이템은 종류(type)별로 티어(tier) 순서대로 구매해야 합니다.
 * - weapon:    훈련용 목검(1) → 강철 세이버(2) → 앱솔랩스 소드(3)
 * - armor:     수습 도복(1)   → 네크로 아머(2) → 카루타 세트(3)
 * - accessory: 실버블라썸 링(1) → 보스 장신구 세트(2)
 *
 * tier 4 이상의 아이템은 대장간(craftRecipes.js)에서만 제작 가능합니다.
 */

/** @type {Array<{ id: string, name: string, desc: string, cost: number, type: string, tier: number, stat: Object }>} */
export const SHOP_ITEMS = [
  // 무기 (weapon)
  { id: 'w1', name: '훈련용 목검',    desc: 'STR+10, DEX+5',       cost: 100,  type: 'weapon',    tier: 1, stat: { str: 10, dex: 5 } },
  { id: 'w2', name: '강철 세이버',    desc: 'STR+40, DEX+20',      cost: 500,  type: 'weapon',    tier: 2, stat: { str: 40, dex: 20 } },
  { id: 'w3', name: '앱솔랩스 소드',  desc: 'STR+120, DEX+60',     cost: 2500, type: 'weapon',    tier: 3, stat: { str: 120, dex: 60 } },

  // 방어구 (armor)
  { id: 'a1', name: '수습 도복',      desc: 'HP+200, MP+100',      cost: 150,  type: 'armor',     tier: 1, stat: { maxHp: 200, maxMp: 100 } },
  { id: 'a2', name: '네크로 아머',    desc: 'HP+1000, MP+400',     cost: 800,  type: 'armor',     tier: 2, stat: { maxHp: 1000, maxMp: 400 } },
  { id: 'a3', name: '카루타 세트',    desc: 'HP+4000, MP+2000',    cost: 3000, type: 'armor',     tier: 3, stat: { maxHp: 4000, maxMp: 2000 } },

  // 장신구 (accessory)
  { id: 'ac1', name: '실버블라썸 링',   desc: 'AllStat+5',  cost: 300,  type: 'accessory', tier: 1, stat: { str: 5, dex: 5, int: 5, luk: 5 } },
  { id: 'ac2', name: '보스 장신구 세트', desc: 'AllStat+20', cost: 1200, type: 'accessory', tier: 2, stat: { str: 20, dex: 20, int: 20, luk: 20 } },
];
