/**
 * 전투 계산 유틸리티
 *
 * 주사위 평가(Yahtzee 방식)와 스킬 보정값 집계를 담당합니다.
 * 모두 순수 함수로 사이드이펙트 없음.
 */

/**
 * 주사위 배열을 평가하여 콤보 이름과 배율을 반환합니다.
 *
 * 플레이어는 5개 주사위, 몬스터는 3개 주사위를 사용합니다.
 * 콤보 판정 기준: Yahtzee 포커 패 (5개) / 소형 포커 패 (3개)
 *
 * @param {number[]} diceArray - 주사위 눈금 배열
 * @returns {{ name: string, mult: number, sum: number }}
 */
export const evaluateDice = (diceArray) => {
  const counts = {};
  let sum = 0;

  diceArray.forEach(d => {
    counts[d] = (counts[d] || 0) + 1;
    sum += d;
  });

  const values     = Object.values(counts).sort((a, b) => b - a); // 내림차순 (e.g. [3, 2])
  const uniqueFaces = Object.keys(counts).map(Number).sort((a, b) => a - b);

  // 스트레이트 판정: 연속된 숫자인지 확인
  let isStraight = false;
  if (diceArray.length === 5 && uniqueFaces.length === 5) {
    isStraight = uniqueFaces[4] - uniqueFaces[0] === 4;
  } else if (diceArray.length === 3 && uniqueFaces.length === 3) {
    isStraight = uniqueFaces[2] - uniqueFaces[0] === 2;
  }

  // 5개 주사위 콤보표
  if (diceArray.length === 5) {
    if (values[0] === 5)                              return { name: '5 of a Kind!', mult: 5.0, sum };
    if (isStraight)                                   return { name: 'Straight!',    mult: 2.5, sum };
    if (values[0] === 4)                              return { name: '4 of a Kind!', mult: 3.0, sum };
    if (values[0] === 3 && values[1] === 2)           return { name: 'Full House!',  mult: 2.5, sum };
    if (values[0] === 3)                              return { name: '3 of a Kind!', mult: 2.0, sum };
    if (values[0] === 2 && values[1] === 2)           return { name: 'Two Pairs!',   mult: 1.5, sum };
    if (values[0] === 2)                              return { name: '1 Pair!',       mult: 1.2, sum };
    return { name: 'No Combo', mult: 1.0, sum };
  }

  // 3개 주사위 콤보표 (몬스터용)
  if (values[0] === 3)  return { name: '3 of a Kind!', mult: 3.0, sum };
  if (isStraight)       return { name: 'Straight!',    mult: 2.0, sum };
  if (values[0] === 2)  return { name: '1 Pair!',      mult: 1.5, sum };
  return { name: 'No Combo', mult: 1.0, sum };
};

/**
 * 플레이어의 스킬 목록을 순회하여 전투 보정값을 집계합니다.
 *
 * @param {Array<{ id: string, level: number }>} skills - 플레이어 스킬 배열
 * @returns {{
 *   extraFlatDmg:  number,  // 추가 고정 피해
 *   extraDmgReduc: number,  // 피해 감소 (음수면 방어 손실)
 *   extraDiceSum:  number,  // 주사위 합 추가 보정
 *   extraMult:     number,  // 피해 배율 추가 보정
 *   lifesteal:     number,  // 피해량 대비 회복 비율 (0.0~1.0)
 * }}
 */
export const calcSkillEffects = (skills = []) => {
  let extraFlatDmg  = 0;
  let extraDmgReduc = 0;
  let extraDiceSum  = 0;
  let extraMult     = 0;
  let lifesteal     = 0;

  skills.forEach(s => {
    if (s.level === 0) return; // 미학습 스킬은 효과 없음

    switch (s.id) {
      // 용사 스킬
      case 'hero_t1':     extraFlatDmg  += 20  * s.level; break;
      case 'hero_t2':     extraDmgReduc += 15  * s.level; break;
      case 'hero_ult':    extraMult     += 1.0 * s.level; break;
      // 타짜 스킬 (잭팟: 배율 폭증 대신 방어 포기)
      case 'gambler_t1':  extraFlatDmg  += 15  * s.level; break;
      case 'gambler_t2':  extraMult     += 0.5 * s.level; break;
      case 'gambler_ult': extraMult     += 2.0 * s.level; extraDmgReduc -= 50; break;
      // 광전사 스킬
      case 'berserk_t1':  extraFlatDmg  += 25  * s.level; break;
      case 'berserk_t2':  lifesteal     += 0.1 * s.level; break;
      case 'berserk_ult': extraDmgReduc += 30  * s.level; break;
      // 대도 스킬
      case 'thief_t1':    extraFlatDmg  += 15  * s.level; break;
      case 'thief_t2':    extraDmgReduc += 10  * s.level; break;
      case 'thief_ult':   extraMult     += 0.5 * s.level; break;
      // 정령술사 스킬
      case 'elem_t1':     extraFlatDmg  += 18  * s.level; break;
      case 'elem_t2':     extraDmgReduc += 12  * s.level; break;
      case 'elem_ult':    extraMult     += 1.0 * s.level; break;
      // 전사/검투사 스킬
      case 'warrior_t1':  extraFlatDmg  += 10  * s.level; break;
      case 'warrior_t2':  extraDiceSum  += 5   * s.level; break;
      case 'warrior_ult': extraMult     += 0.5 * s.level; break;
      // 도적/암살자 스킬
      case 'rogue_t1':    extraFlatDmg  += 12  * s.level; break;
      case 'rogue_t2':    extraDmgReduc += 8   * s.level; break;
      case 'rogue_ult':   extraMult     += 0.5 * s.level; break;
      // 마법사/성기사 스킬 (메테오: 고정 추가 피해)
      case 'mage_t1':     extraFlatDmg  += 15  * s.level; break;
      case 'mage_t2':     extraDmgReduc += 10  * s.level; break;
      case 'mage_ult':    extraFlatDmg  += 50  * s.level; break;
      // 백수 스킬
      case 'jobless_t1':  extraFlatDmg  += 5   * s.level; break;
      case 'jobless_t2':  extraDmgReduc += 5   * s.level; break;
      case 'jobless_ult': extraDiceSum  += 10  * s.level; break;
    }
  });

  return { extraFlatDmg, extraDmgReduc, extraDiceSum, extraMult, lifesteal };
};
