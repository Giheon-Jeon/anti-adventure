/**
 * 유니온 보너스 시스템
 *
 * 비활성 슬롯의 캐릭터 레벨·직업에 따라 현재 플레이어에게
 * 추가 능력치(스탯, 최대 HP)를 부여합니다.
 * - 레벨 5마다 모든 스탯 +1
 * - 레벨당 최대 HP +5
 * - 특정 직업 계열은 해당 스탯에 추가 보너스
 */

/**
 * 현재 게임 상태를 기반으로 비활성 슬롯의 유니온 보너스를 계산합니다.
 *
 * @param {{ characters: Array, activeSlot: number }} state
 * @returns {{ str: number, dex: number, int: number, luk: number, maxHp: number, totalLevel: number }}
 */
export const getUnionBonus = (state) => {
  const bonus = { str: 0, dex: 0, int: 0, luk: 0, maxHp: 0, totalLevel: 0 };
  if (!state.characters) return bonus;

  state.characters.forEach((char, idx) => {
    // 현재 활성 슬롯은 자기 자신이므로 유니온 대상 제외
    if (!char || idx === state.activeSlot) return;

    bonus.totalLevel += char.level;

    // 레벨 기반 스탯 보너스 (5레벨마다 모든 스탯 +1)
    const levelBonus = Math.floor(char.level / 5);
    bonus.str   += levelBonus;
    bonus.dex   += levelBonus;
    bonus.int   += levelBonus;
    bonus.luk   += levelBonus;
    bonus.maxHp += char.level * 5;

    // 직업 계열별 추가 보너스
    const { job } = char;
    if (job === 'Novice') bonus.maxHp += 10; // 초보자는 HP 소량 추가
    if (job.includes('전사') || job.includes('Warrior'))                            bonus.str += 5;
    if (job.includes('도적') || job.includes('대도') || job.includes('Rogue'))     bonus.dex += 5;
    if (job.includes('마법사') || job.includes('정령술사') || job.includes('Mage')) bonus.int += 5;
    if (job.includes('타짜') || job.includes('Gambler'))                           bonus.luk += 10;
  });

  return bonus;
};
