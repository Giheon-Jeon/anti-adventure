/**
 * 직업(Job) 시스템
 *
 * 직업 등급: Common → Rare → Hero → Legendary
 * 10레벨 달성 시 evaluateJob()을 통해 조건에 맞는 직업으로 전직합니다.
 * 각 직업은 getJobSkills()로 전용 스킬 3종(1차기 / 2차기 / 궁극기)을 받습니다.
 */

/**
 * 플레이어 상태를 기반으로 전직 직업을 결정합니다.
 * 높은 등급(Legendary)부터 순서대로 검사하며, 처음 충족되는 직업을 반환합니다.
 *
 * @param {Object} player - 현재 플레이어 객체
 * @returns {{ job: string, desc: string, bonus: Object }}
 */
export const evaluateJob = (player) => {
  const { stats, weapon_tier, armor_tier, accessory_tier, statistics, gold } = player;

  // ── Legendary ────────────────────────────────────────────────────────────
  // 모든 스탯 30 이상 + 무기·방어구 3티어 → 용사
  if (stats.str >= 30 && stats.dex >= 30 && stats.int >= 30 && stats.luk >= 30
      && weapon_tier === 3 && armor_tier === 3) {
    return {
      job: '용사 (Legendary)',
      desc: '모든 분야에 통달한 세계의 구원자',
      bonus: { str: 20, dex: 20, int: 20, luk: 20 },
    };
  }
  // 장비 없이 LUK 50 → 타짜 (무기·방어구를 구매하면 조건 실패)
  if (stats.luk >= 50 && armor_tier === 0 && weapon_tier === 0) {
    return {
      job: '타짜 (Legendary)',
      desc: '운에 모든 것을 건 전설의 도박사',
      bonus: { luk: 50, dex: 10 },
    };
  }

  // ── Hero ─────────────────────────────────────────────────────────────────
  // 근사치 사망 경험 + 다수 처치 + 고 STR → 광전사
  if (stats.str >= 40 && statistics.lowestHpPercent <= 10 && statistics.monstersKilled >= 20) {
    return {
      job: '광전사 (Hero)',
      desc: '죽음의 문턱에서 각성한 전사',
      bonus: { str: 30, maxHp: 300 },
    };
  }
  // 부(골드) + 민첩 + 행운 → 대도
  if (stats.dex >= 30 && stats.luk >= 30 && gold >= 3000) {
    return {
      job: '대도 (Hero)',
      desc: '돈과 운이 따르는 전설적인 도둑',
      bonus: { dex: 20, luk: 20 },
    };
  }
  // 포션 소비 + 고 INT → 정령술사
  if (stats.int >= 40 && statistics.potionsUsed >= 10 && statistics.monstersKilled >= 10) {
    return {
      job: '정령술사 (Hero)',
      desc: '마력을 한계치까지 끌어올린 자',
      bonus: { int: 30, maxMp: 200 },
    };
  }

  // ── Rare ─────────────────────────────────────────────────────────────────
  if (stats.str >= 30 && weapon_tier >= 2)
    return { job: '검투사 (Rare)', desc: '무기 다루는 법을 마스터한 투사', bonus: { str: 15 } };
  if (stats.dex >= 30 && accessory_tier >= 2)
    return { job: '암살자 (Rare)', desc: '어둠 속에 숨어 적을 노리는 자', bonus: { dex: 15 } };
  if (stats.str >= 25 && stats.int >= 25 && armor_tier >= 2)
    return { job: '성기사 (Rare)', desc: '신성한 힘을 다루는 기사', bonus: { str: 10, int: 10, maxHp: 100 } };

  // ── Common ────────────────────────────────────────────────────────────────
  if (stats.str >= 25) return { job: '전사 (Common)',  desc: '힘을 바탕으로 싸우는 자',  bonus: { str: 10 } };
  if (stats.dex >= 25) return { job: '도적 (Common)',  desc: '민첩함을 무기로 삼는 자',  bonus: { dex: 10 } };
  if (stats.int >= 25) return { job: '마법사 (Common)', desc: '마력을 다루는 자',        bonus: { int: 10 } };

  // 아무 조건도 충족하지 못한 경우
  return { job: '백수 (Common)', desc: '아무것도 하지 않은 자', bonus: { luk: 5 } };
};

/**
 * 직업 이름에 해당하는 전용 스킬 3종을 반환합니다.
 * 스킬 구성: 1차기(t1) / 2차기(t2) / 궁극기(ult)
 *
 * @param {string} jobName - 직업 이름 (예: '전사 (Common)', '용사 (Legendary)')
 * @returns {Array<Object>} 스킬 객체 배열
 */
export const getJobSkills = (jobName) => {
  const t1  = { level: 0, maxLevel: 5 }; // 1차기 기본 템플릿
  const t2  = { level: 0, maxLevel: 5 }; // 2차기 기본 템플릿
  const ult = { level: 0, maxLevel: 1 }; // 궁극기 (단 1회만 찍을 수 있음)

  if (jobName.includes('용사')) return [
    { id: 'hero_t1',  name: '빛의 검 (1차)',   desc: '최종 피해량 +20',   ...t1  },
    { id: 'hero_t2',  name: '신의 가호 (2차)',  desc: '받는 피해량 -15',  ...t2  },
    { id: 'hero_ult', name: '초신성 (궁극기)',  desc: '피해량 2배 폭증',  ...ult },
  ];
  if (jobName.includes('타짜')) return [
    { id: 'gambler_t1',  name: '밑장 빼기 (1차)', desc: '추가 피해 +15',           ...t1  },
    { id: 'gambler_t2',  name: '올인 (2차)',       desc: '주사위 배율 +0.5배',      ...t2  },
    { id: 'gambler_ult', name: '잭팟 (궁극기)',    desc: '피해량 3배 폭증, 방어 0', ...ult },
  ];
  if (jobName.includes('광전사')) return [
    { id: 'berserk_t1',  name: '분노의 일격 (1차)', desc: '추가 피해 +25',              ...t1  },
    { id: 'berserk_t2',  name: '피의 굶주림 (2차)', desc: '적에게 가한 피해 일부 회복', ...t2  },
    { id: 'berserk_ult', name: '불사 (궁극기)',      desc: '받는 피해 대폭 감소',        ...ult },
  ];
  if (jobName.includes('대도')) return [
    { id: 'thief_t1',  name: '동전 던지기 (1차)', desc: '피해량 +15',          ...t1  },
    { id: 'thief_t2',  name: '회피 기동 (2차)',   desc: '받는 피해량 -10',    ...t2  },
    { id: 'thief_ult', name: '그림자 분신 (궁극기)', desc: '피해량 1.5배 증폭', ...ult },
  ];
  if (jobName.includes('정령술사')) return [
    { id: 'elem_t1',  name: '정령 소환 (1차)',      desc: '추가 피해 +18',     ...t1  },
    { id: 'elem_t2',  name: '원소 방패 (2차)',      desc: '받는 피해 -12',    ...t2  },
    { id: 'elem_ult', name: '정령의 분노 (궁극기)', desc: '피해량 2배 폭증',  ...ult },
  ];
  // 검투사와 전사는 동일 스킬셋 공유
  if (jobName.includes('검투사') || jobName.includes('전사')) return [
    { id: 'warrior_t1',  name: '강타 (1차)',        desc: '최종 피해량 +10',            ...t1  },
    { id: 'warrior_t2',  name: '돌진 (2차)',        desc: '주사위 눈금 합에 +5 보너스', ...t2  },
    { id: 'warrior_ult', name: '회전베기 (궁극기)', desc: '최종 피해량 1.5배 증폭',     ...ult },
  ];
  // 암살자와 도적은 동일 스킬셋 공유
  if (jobName.includes('암살자') || jobName.includes('도적')) return [
    { id: 'rogue_t1',  name: '급소 찌르기 (1차)', desc: '최종 피해량 +12',    ...t1  },
    { id: 'rogue_t2',  name: '그림자 숨기 (2차)', desc: '받는 피해량 8 감소', ...t2  },
    { id: 'rogue_ult', name: '암살 (궁극기)',      desc: '주사위 배율 +0.5배', ...ult },
  ];
  // 성기사와 마법사는 동일 스킬셋 공유
  if (jobName.includes('성기사') || jobName.includes('마법사')) return [
    { id: 'mage_t1',  name: '매직 클로 (1차)',  desc: '최종 피해량 +15',          ...t1  },
    { id: 'mage_t2',  name: '마력 방패 (2차)',  desc: '받는 피해량 10 감소',      ...t2  },
    { id: 'mage_ult', name: '메테오 (궁극기)',  desc: '모든 공격에 50 고정 피해', ...ult },
  ];

  // 백수(Jobless) 기본 스킬
  return [
    { id: 'jobless_t1',  name: '돌 던지기 (1차)',    desc: '최종 피해량 +5',             ...t1  },
    { id: 'jobless_t2',  name: '죽은 척하기 (2차)',  desc: '받는 피해량 5 감소',         ...t2  },
    { id: 'jobless_ult', name: '대박 기원 (궁극기)', desc: '주사위 결과 무조건 +10 추가', ...ult },
  ];
};
