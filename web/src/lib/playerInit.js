/**
 * 플레이어 초기 상태 정의
 *
 * 새 캐릭터 생성 및 슬롯 리셋 시 사용됩니다.
 * 스탯·스킬의 초기값을 한 곳에서 관리합니다.
 */

/**
 * 새 플레이어 객체를 반환합니다.
 * @param {string} [name='Adventurer'] - 캐릭터 이름
 * @returns {Object} 초기 플레이어 객체
 */
export const getInitialPlayer = (name = 'Adventurer') => ({
  name,
  level: 1,
  job: 'Novice',
  hp: 100, maxHp: 100,
  mp: 50,  maxMp: 50,
  exp: 0,  maxExp: 100,
  gold: 500,

  // 장비 티어 (0 = 미구매, 최대 3~4)
  weapon_tier: 0, armor_tier: 0, accessory_tier: 0,

  stats: { str: 10, dex: 10, int: 10, luk: 10 },
  skill_points: 0,

  // 기본 스킬 3종 (전직 전 공용)
  skills: [
    { id: 'power_strike', name: '파워 스트라이크', level: 0, maxLevel: 5, desc: '적에게 가하는 최종 피해량이 레벨당 5 증가합니다.' },
    { id: 'iron_skin',    name: '아이언 스킨',    level: 0, maxLevel: 5, desc: '적에게 받는 최종 피해량이 레벨당 3 감소합니다.' },
    { id: 'lucky_dice',   name: '행운의 주사위',  level: 0, maxLevel: 5, desc: '내 주사위 눈금이 레벨당 1씩 추가로 보정됩니다.' },
  ],

  inventory: [],
  needsRepair: false,

  // 전직 조건 판별에 사용되는 플레이 통계
  statistics: {
    monstersKilled:   0,
    timesWorked:      0,
    potionsUsed:      0,
    moneySpent:       0,
    lowestHpPercent: 100, // HP가 가장 낮았던 % (광전사 전직 조건)
  },
});

/**
 * 게임 전체 초기 상태 (3슬롯 + 로그 + 도감 + 던전 목록)
 *
 * 던전 목록은 항상 이 값으로 덮어씌워지므로 세이브 데이터와 무관하게 최신 상태가 유지됩니다.
 */
export const initialState = {
  characters: [getInitialPlayer('Player 1'), null, null],
  activeSlot: 0,
  logs: ['Welcome to Anti-Adventure Web!'],
  encyclopedia: [],
  dungeons: [
    { id: 'field',  name: '초심자의 들판', recLevel: 1,  type: 'easy'   },
    { id: 'forest', name: '어두운 숲',     recLevel: 5,  type: 'normal' },
    { id: 'cave',   name: '고블린 동굴',   recLevel: 10, type: 'hard'   },
    { id: 'ruins',  name: '저주받은 폐허', recLevel: 15, type: 'hard'   },
    { id: 'abyss',  name: '심연의 나락',   recLevel: 20, type: 'expert' },
    { id: 'dragon', name: '드래곤의 둥지', recLevel: 30, type: 'hell'   },
  ],
};
