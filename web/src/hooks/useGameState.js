import { useState, useCallback, useEffect } from 'react';

const getInitialPlayer = (name = 'Adventurer') => ({
  name,
  level: 1,
  job: 'Novice',
  hp: 100, maxHp: 100,
  mp: 50, maxMp: 50,
  exp: 0, maxExp: 100,
  gold: 500,
  weapon_tier: 0, armor_tier: 0, accessory_tier: 0,
  stats: { str: 10, dex: 10, int: 10, luk: 10 },
  skill_points: 0,
  skills: [
    { id: 'power_strike', name: '파워 스트라이크', level: 0, maxLevel: 5, desc: '적에게 가하는 최종 피해량이 레벨당 5 증가합니다.' },
    { id: 'iron_skin', name: '아이언 스킨', level: 0, maxLevel: 5, desc: '적에게 받는 최종 피해량이 레벨당 3 감소합니다.' },
    { id: 'lucky_dice', name: '행운의 주사위', level: 0, maxLevel: 5, desc: '내 주사위 눈금이 레벨당 1씩 추가로 보정됩니다.' }
  ],
  inventory: [],
  needsRepair: false,
  statistics: { monstersKilled: 0, timesWorked: 0, potionsUsed: 0, moneySpent: 0, lowestHpPercent: 100 }
});

const initialState = {
  characters: [
    getInitialPlayer('Player 1'),
    null,
    null
  ],
  activeSlot: 0,
  logs: ['Welcome to Anti-Adventure Web!'],
  encyclopedia: [],
  dungeons: [
    { id: 'field', name: '초심자의 들판', recLevel: 1, type: 'easy' },
    { id: 'forest', name: '어두운 숲', recLevel: 5, type: 'normal' },
    { id: 'cave', name: '고블린 동굴', recLevel: 10, type: 'hard' },
    { id: 'ruins', name: '저주받은 폐허', recLevel: 15, type: 'hard' },
    { id: 'abyss', name: '심연의 나락', recLevel: 20, type: 'expert' },
    { id: 'dragon', name: '드래곤의 둥지', recLevel: 30, type: 'hell' },
  ],
};

const evaluateJob = (player) => {
  const { stats, weapon_tier, armor_tier, accessory_tier, statistics, gold } = player;
  
  // Legendary (레전더리)
  if (stats.str >= 30 && stats.dex >= 30 && stats.int >= 30 && stats.luk >= 30 && weapon_tier === 3 && armor_tier === 3) {
    return { job: '용사 (Legendary)', desc: '모든 분야에 통달한 세계의 구원자', bonus: { str: 20, dex: 20, int: 20, luk: 20 } };
  }
  if (stats.luk >= 50 && armor_tier === 0 && weapon_tier === 0) {
    return { job: '타짜 (Legendary)', desc: '운에 모든 것을 건 전설의 도박사', bonus: { luk: 50, dex: 10 } };
  }
  
  // Hero (영웅)
  if (stats.str >= 40 && statistics.lowestHpPercent <= 10 && statistics.monstersKilled >= 20) {
    return { job: '광전사 (Hero)', desc: '죽음의 문턱에서 각성한 전사', bonus: { str: 30, maxHp: 300 } };
  }
  if (stats.dex >= 30 && stats.luk >= 30 && gold >= 3000) {
    return { job: '대도 (Hero)', desc: '돈과 운이 따르는 전설적인 도둑', bonus: { dex: 20, luk: 20 } };
  }
  if (stats.int >= 40 && statistics.potionsUsed >= 10 && statistics.monstersKilled >= 10) {
    return { job: '정령술사 (Hero)', desc: '마력을 한계치까지 끌어올린 자', bonus: { int: 30, maxMp: 200 } };
  }
  
  // Rare (희귀)
  if (stats.str >= 30 && weapon_tier >= 2) return { job: '검투사 (Rare)', desc: '무기 다루는 법을 마스터한 투사', bonus: { str: 15 } };
  if (stats.dex >= 30 && accessory_tier >= 2) return { job: '암살자 (Rare)', desc: '어둠 속에 숨어 적을 노리는 자', bonus: { dex: 15 } };
  if (stats.str >= 25 && stats.int >= 25 && armor_tier >= 2) return { job: '성기사 (Rare)', desc: '신성한 힘을 다루는 기사', bonus: { str: 10, int: 10, maxHp: 100 } };
  
  // Common (일반)
  if (stats.str >= 25) return { job: '전사 (Common)', desc: '힘을 바탕으로 싸우는 자', bonus: { str: 10 } };
  if (stats.dex >= 25) return { job: '도적 (Common)', desc: '민첩함을 무기로 삼는 자', bonus: { dex: 10 } };
  if (stats.int >= 25) return { job: '마법사 (Common)', desc: '마력을 다루는 자', bonus: { int: 10 } };
  
  return { job: '백수 (Common)', desc: '아무것도 하지 않은 자', bonus: { luk: 5 } };
};

export const getJobSkills = (jobName) => {
  const t1 = { level: 0, maxLevel: 5 };
  const t2 = { level: 0, maxLevel: 5 };
  const ult = { level: 0, maxLevel: 1 };
  
  if (jobName.includes('용사')) return [
    { id: 'hero_t1', name: '빛의 검 (1차)', desc: '최종 피해량 +20', ...t1 },
    { id: 'hero_t2', name: '신의 가호 (2차)', desc: '받는 피해량 -15', ...t2 },
    { id: 'hero_ult', name: '초신성 (궁극기)', desc: '피해량 2배 폭증', ...ult }
  ];
  if (jobName.includes('타짜')) return [
    { id: 'gambler_t1', name: '밑장 빼기 (1차)', desc: '추가 피해 +15', ...t1 },
    { id: 'gambler_t2', name: '올인 (2차)', desc: '주사위 배율 +0.5배', ...t2 },
    { id: 'gambler_ult', name: '잭팟 (궁극기)', desc: '피해량 3배 폭증, 방어 0', ...ult }
  ];
  if (jobName.includes('광전사')) return [
    { id: 'berserk_t1', name: '분노의 일격 (1차)', desc: '추가 피해 +25', ...t1 },
    { id: 'berserk_t2', name: '피의 굶주림 (2차)', desc: '적에게 가한 피해 일부 회복', ...t2 },
    { id: 'berserk_ult', name: '불사 (궁극기)', desc: '받는 피해 대폭 감소', ...ult }
  ];
  if (jobName.includes('대도')) return [
    { id: 'thief_t1', name: '동전 던지기 (1차)', desc: '피해량 +15', ...t1 },
    { id: 'thief_t2', name: '회피 기동 (2차)', desc: '받는 피해량 -10', ...t2 },
    { id: 'thief_ult', name: '그림자 분신 (궁극기)', desc: '피해량 1.5배 증폭', ...ult }
  ];
  if (jobName.includes('정령술사')) return [
    { id: 'elem_t1', name: '정령 소환 (1차)', desc: '추가 피해 +18', ...t1 },
    { id: 'elem_t2', name: '원소 방패 (2차)', desc: '받는 피해 -12', ...t2 },
    { id: 'elem_ult', name: '정령의 분노 (궁극기)', desc: '피해량 2배 폭증', ...ult }
  ];
  if (jobName.includes('검투사') || jobName.includes('전사')) return [
    { id: 'warrior_t1', name: '강타 (1차)', desc: '최종 피해량 +10', ...t1 },
    { id: 'warrior_t2', name: '돌진 (2차)', desc: '주사위 눈금 합에 +5 보너스', ...t2 },
    { id: 'warrior_ult', name: '회전베기 (궁극기)', desc: '최종 피해량 1.5배 증폭', ...ult }
  ];
  if (jobName.includes('암살자') || jobName.includes('도적')) return [
    { id: 'rogue_t1', name: '급소 찌르기 (1차)', desc: '최종 피해량 +12', ...t1 },
    { id: 'rogue_t2', name: '그림자 숨기 (2차)', desc: '받는 피해량 8 감소', ...t2 },
    { id: 'rogue_ult', name: '암살 (궁극기)', desc: '주사위 배율 +0.5배', ...ult }
  ];
  if (jobName.includes('성기사') || jobName.includes('마법사')) return [
    { id: 'mage_t1', name: '매직 클로 (1차)', desc: '최종 피해량 +15', ...t1 },
    { id: 'mage_t2', name: '마력 방패 (2차)', desc: '받는 피해량 10 감소', ...t2 },
    { id: 'mage_ult', name: '메테오 (궁극기)', desc: '모든 공격에 50 고정 피해', ...ult }
  ];
  
  return [
    { id: 'jobless_t1', name: '돌 던지기 (1차)', desc: '최종 피해량 +5', ...t1 },
    { id: 'jobless_t2', name: '죽은 척하기 (2차)', desc: '받는 피해량 5 감소', ...t2 },
    { id: 'jobless_ult', name: '대박 기원 (궁극기)', desc: '주사위 결과 무조건 +10 추가', ...ult }
  ];
};

export const getUnionBonus = (state) => {
  let bonus = { str: 0, dex: 0, int: 0, luk: 0, maxHp: 0, totalLevel: 0 };
  if (!state.characters) return bonus;
  
  state.characters.forEach((char, idx) => {
    if (char && idx !== state.activeSlot) {
      bonus.totalLevel += char.level;
      const levelBonus = Math.floor(char.level / 5);
      bonus.str += levelBonus;
      bonus.dex += levelBonus;
      bonus.int += levelBonus;
      bonus.luk += levelBonus;
      bonus.maxHp += char.level * 5;
      
      // Job specific bonuses
      if (char.job === 'Novice') bonus.maxHp += 10;
      if (char.job.includes('Warrior') || char.job.includes('전사')) bonus.str += 5;
      if (char.job.includes('Rogue') || char.job.includes('도적') || char.job.includes('대도')) bonus.dex += 5;
      if (char.job.includes('Mage') || char.job.includes('마법사') || char.job.includes('정령술사')) bonus.int += 5;
      if (char.job.includes('Gambler') || char.job.includes('타짜')) bonus.luk += 10;
    }
  });
  return bonus;
};

export function useGameState() {
  const [gameState, setGameState] = useState(() => {
    const saved = localStorage.getItem('anti-adventure-save');
    if (saved) {
      try { 
        const parsed = JSON.parse(saved);
        
        // Migration from old schema (single player) to multiple slots
        if (!parsed.characters) {
          const oldPlayer = parsed.player || getInitialPlayer('Player 1');
          if (!oldPlayer.skills) oldPlayer.skills = getInitialPlayer().skills;
          if (oldPlayer.skill_points === undefined) oldPlayer.skill_points = 0;
          if (!oldPlayer.statistics) oldPlayer.statistics = { monstersKilled: 0, timesWorked: 0, potionsUsed: 0, moneySpent: 0, lowestHpPercent: 100 };
          parsed.characters = [oldPlayer, null, null];
          parsed.activeSlot = 0;
        } else {
          // Migrate old slots
          parsed.characters.forEach(char => {
            if (char && !char.statistics) char.statistics = { monstersKilled: 0, timesWorked: 0, potionsUsed: 0, moneySpent: 0, lowestHpPercent: 100 };
          });
        }

        return parsed;
      } catch (e) { return initialState; }
    }
    return initialState;
  });

  useEffect(() => {
    localStorage.setItem('anti-adventure-save', JSON.stringify(gameState));
  }, [gameState]);

  const dispatch = useCallback((action) => {
    setGameState((prev) => {
      let newState = JSON.parse(JSON.stringify(prev));
      let player = newState.characters[newState.activeSlot];
      
      const checkJobAdvancement = () => {
        if (player.level >= 10 && player.job === 'Novice') {
          const jobResult = evaluateJob(player);
          player.job = jobResult.job;
          if (jobResult.bonus.str) player.stats.str += jobResult.bonus.str;
          if (jobResult.bonus.dex) player.stats.dex += jobResult.bonus.dex;
          if (jobResult.bonus.int) player.stats.int += jobResult.bonus.int;
          if (jobResult.bonus.luk) player.stats.luk += jobResult.bonus.luk;
          if (jobResult.bonus.maxHp) { player.maxHp += jobResult.bonus.maxHp; player.hp = player.maxHp + getUnionBonus(newState).maxHp; }
          if (jobResult.bonus.maxMp) { player.maxMp += jobResult.bonus.maxMp; player.mp = player.maxMp; }
          
          const newSkills = getJobSkills(jobResult.job);
          player.skills.push(...newSkills);
          
          newState.logs = [
            `🎉 [전직 완료!] ${jobResult.desc}`,
            `✨ 10레벨 달성! 새로운 직업 '${jobResult.job}'을(를) 얻었습니다! (전용 스킬 3종 획득)`, 
            ...newState.logs
          ].slice(0, 50);
        }
      };

      switch (action.type) {
        case 'WORK_JOB':
          if (player.hp > 15) {
            const roll = Math.floor(Math.random() * 100);
            let gold = 0, hpLoss = 0, msg = '';
            if (roll < 10) { gold = 40; hpLoss = -10; msg = '✨ [대성공!] 사람들에게 칭찬을 들었습니다! (+40G, HP회복 10)'; }
            else if (roll < 60) { gold = 25; hpLoss = 5; msg = '✅ [성공] 무난하게 청소를 마쳤습니다. (+25G, HP감소 5)'; }
            else if (roll < 90) { gold = 10; hpLoss = 15; msg = '❓ [실패] 청소가 덜 된 곳이 있습니다. (+10G, HP감소 15)'; }
            else { gold = 0; hpLoss = 25; msg = '💀 [대실패] 허리를 삐끗했습니다! (+0G, HP감소 25)'; }
            
            player.gold += gold;
            player.hp -= hpLoss;
            const maxHpWithUnion = player.maxHp + getUnionBonus(newState).maxHp;
            if (player.hp > maxHpWithUnion) player.hp = maxHpWithUnion;
            if (player.hp < 1) player.hp = 1;
            player.statistics.timesWorked += 1;
            
            const hpPct = (player.hp / maxHpWithUnion) * 100;
            if (hpPct < player.statistics.lowestHpPercent) player.statistics.lowestHpPercent = hpPct;
            
            newState.logs = [msg, ...newState.logs].slice(0, 50);
          } else {
            newState.logs = ['체력이 부족하여 알바를 할 수 없습니다. (HP 15 초과 필요)', ...newState.logs].slice(0, 50);
          }
          break;

        case 'BUY_ITEM':
          const { cost, statIncreases, newTier, type } = action.payload;
          if (player.gold >= cost) {
            player.gold -= cost;
            player.statistics.moneySpent += cost;
            if (statIncreases.str) player.stats.str += statIncreases.str;
            if (statIncreases.dex) player.stats.dex += statIncreases.dex;
            if (statIncreases.int) player.stats.int += statIncreases.int;
            if (statIncreases.luk) player.stats.luk += statIncreases.luk;
            if (statIncreases.maxHp) { 
              player.maxHp += statIncreases.maxHp; 
              player.hp = player.maxHp + getUnionBonus(newState).maxHp; 
            }
            if (statIncreases.maxMp) { player.maxMp += statIncreases.maxMp; player.mp = player.maxMp; }
            if (type === 'weapon') player.weapon_tier = newTier;
            if (type === 'armor') player.armor_tier = newTier;
            if (type === 'accessory') player.accessory_tier = newTier;
            newState.logs = [`상점에서 장비를 구매했습니다! (-${cost}G)`, ...newState.logs].slice(0, 50);
          } else {
            newState.logs = [`장비를 구매할 골드가 부족합니다! (${cost}G 필요)`, ...newState.logs].slice(0, 50);
          }
          break;

        case 'BUY_POTION':
          if (player.gold >= 30) {
            player.gold -= 30;
            player.statistics.moneySpent += 30;
            player.statistics.potionsUsed += 1;
            if (action.payload === 'hp') {
              const maxHpWithUnion = player.maxHp + getUnionBonus(newState).maxHp;
              player.hp = Math.min(maxHpWithUnion, player.hp + Math.floor(player.maxHp / 2));
              newState.logs = ['빨간 포션을 마셔 체력을 회복했습니다.', ...newState.logs].slice(0, 50);
            } else {
              player.mp = Math.min(player.maxMp, player.mp + Math.floor(player.maxMp / 2));
              newState.logs = ['파란 포션을 마셔 마나를 회복했습니다.', ...newState.logs].slice(0, 50);
            }
          } else {
            newState.logs = ['포션을 살 골드가 부족합니다! (30G 필요)', ...newState.logs].slice(0, 50);
          }
          break;

        case 'BUY_HINT':
          if (player.gold >= 500) {
            player.gold -= 500;
            const hints = [
              "광전사의 길: 죽음의 문턱(HP 10% 이하)을 경험하고 수많은 몬스터를 벤 강인한 투지(STR)가 필요합니다.",
              "대도의 길: 민첩(DEX)과 행운(LUK)을 갈고닦아 3000G 이상의 부를 축적해야 합니다.",
              "정령술사의 길: 쉴 새 없이 마나포션을 소모하며 지능(INT)의 극의에 달해야 합니다.",
              "전설의 도박사: 아무런 방어구와 무기도 사지 않은 채 자신의 운(LUK 50)만을 믿어야 합니다.",
              "세계의 구원자: 모든 분야(장비 3티어, 모든스탯 30)에 통달해야 전설이 됩니다."
            ];
            const randomHint = hints[Math.floor(Math.random() * hints.length)];
            newState.logs = [
              `📜 정보 상인: "${randomHint}"`,
              ...newState.logs
            ].slice(0, 50);
          } else {
            newState.logs = ['정보를 살 골드가 부족합니다! (500G 필요)', ...newState.logs].slice(0, 50);
          }
          break;
          
        case 'REPAIR_ALL':
          if (!player.needsRepair) {
             newState.logs = ['수리할 장비가 없습니다.', ...newState.logs].slice(0, 50);
             break;
          }
          if (player.gold >= action.payload.cost) {
            player.gold -= action.payload.cost;
            player.needsRepair = false;
            newState.logs = [`모든 장비가 완벽하게 수리되었습니다! (-${action.payload.cost}G)`, ...newState.logs].slice(0, 50);
          } else {
            newState.logs = ['수리비가 부족합니다!', ...newState.logs].slice(0, 50);
          }
          break;

        case 'ADD_LOG':
          newState.logs = [action.payload, ...newState.logs].slice(0, 50);
          break;
          
        case 'EARN_GOLD':
          player.gold += action.payload;
          newState.logs = [`+${action.payload} Gold 획득!`, ...newState.logs].slice(0, 50);
          break;
          
        case 'SPEND_GOLD':
          if (player.gold >= action.payload) {
            player.gold -= action.payload;
            return newState;
          }
          return prev;

        case 'HEAL':
          const maxHpWithUnion = player.maxHp + getUnionBonus(newState).maxHp;
          player.hp = maxHpWithUnion;
          player.mp = player.maxMp;
          newState.logs = ['체력과 마나가 모두 회복되었습니다.', ...newState.logs].slice(0, 50);
          break;

        case 'GAIN_EXP':
          player.exp += action.payload;
          let levelUp = false;
          while (player.exp >= player.maxExp) {
            player.level += 1;
            player.exp -= player.maxExp;
            player.maxExp = Math.floor(player.maxExp * 1.5);
            player.maxHp += 20;
            player.hp = player.maxHp + getUnionBonus(newState).maxHp;
            player.stats.str += 2;
            player.stats.dex += 2;
            player.stats.int += 2;
            player.stats.luk += 2;
            player.skill_points += 1;
            levelUp = true;
          }
          if (levelUp) {
            newState.logs = [`레벨업! 현재 레벨: ${player.level} (+스킬포인트 1)`, ...newState.logs].slice(0, 50);
            checkJobAdvancement();
          }
          break;
          
        case 'COMBAT_WIN':
          const { gold, exp, monsterName } = action.payload;
          player.gold += gold;
          player.exp += exp;
          player.statistics.monstersKilled += 1;
          newState.logs = [`${monsterName} 처치! +${gold}G, +${exp}EXP`, ...newState.logs].slice(0, 50);
          
          let levelUpBattle = false;
          while (player.exp >= player.maxExp) {
            player.level += 1;
            player.exp -= player.maxExp;
            player.maxExp = Math.floor(player.maxExp * 1.5);
            player.maxHp += 20;
            player.hp = player.maxHp + getUnionBonus(newState).maxHp;
            player.stats.str += 2;
            player.stats.dex += 2;
            player.stats.int += 2;
            player.stats.luk += 2;
            player.skill_points += 1;
            levelUpBattle = true;
          }
          if (levelUpBattle) {
            newState.logs = [`레벨업! 현재 레벨: ${player.level} (+스킬포인트 1)`, ...newState.logs].slice(0, 50);
            checkJobAdvancement();
          }
          
          const entry = newState.encyclopedia.find(e => e.name === monsterName);
          if (entry) {
            entry.kills += 1;
          } else {
            newState.encyclopedia.push({ name: monsterName, kills: 1 });
          }
          break;
          
        case 'TRAIN':
          const stat = action.payload;
          player.stats[stat] += 1;
          player.statistics.moneySpent += 50;
          newState.logs = [`훈련을 통해 ${stat.toUpperCase()}이(가) 1 상승했습니다. (-50G)`, ...newState.logs].slice(0, 50);
          break;

        case 'TAKE_DAMAGE':
          player.hp -= action.payload;
          if (player.hp <= 0) {
            player.hp = 1; 
          }
          const maxHpUnion = player.maxHp + getUnionBonus(newState).maxHp;
          const hpPctDmg = (player.hp / maxHpUnion) * 100;
          if (hpPctDmg < player.statistics.lowestHpPercent) player.statistics.lowestHpPercent = hpPctDmg;
          
          if (Math.random() < 0.2) {
            player.needsRepair = true;
          }
          break;
          
        case 'UPGRADE_SKILL':
          const skillId = action.payload;
          if (player.skill_points > 0) {
            const skill = player.skills.find(s => s.id === skillId);
            if (skill && skill.level < skill.maxLevel) {
              skill.level += 1;
              player.skill_points -= 1;
              newState.logs = [`${skill.name} 스킬이 레벨업 되었습니다! (Lv.${skill.level})`, ...newState.logs].slice(0, 50);
            } else {
              newState.logs = ['이미 마스터한 스킬이거나 스킬을 찾을 수 없습니다.', ...newState.logs].slice(0, 50);
            }
          } else {
            newState.logs = ['스킬 포인트가 부족합니다.', ...newState.logs].slice(0, 50);
          }
          break;

        case 'CHANGE_NAME':
          player.name = action.payload;
          newState.logs = [`이름이 ${action.payload}(으)로 변경되었습니다.`, ...newState.logs].slice(0, 50);
          break;

        case 'SWITCH_SLOT':
          const targetSlot = action.payload;
          if (!newState.characters[targetSlot]) {
            newState.characters[targetSlot] = getInitialPlayer(`Player ${targetSlot + 1}`);
            newState.logs = [`새 캐릭터 슬롯(${targetSlot + 1})이 생성되었습니다.`, ...newState.logs].slice(0, 50);
          } else {
            newState.logs = [`캐릭터 슬롯(${targetSlot + 1})으로 전환했습니다.`, ...newState.logs].slice(0, 50);
          }
          newState.activeSlot = targetSlot;
          break;

        case 'RESET_SLOT':
          const resetSlot = action.payload;
          newState.characters[resetSlot] = getInitialPlayer(`Player ${resetSlot + 1}`);
          if (resetSlot === newState.activeSlot) {
            newState.logs = ['현재 플레이 중인 캐릭터가 초기화되었습니다.', ...newState.logs].slice(0, 50);
          } else {
            newState.logs = [`캐릭터 슬롯(${resetSlot + 1})이 초기화되었습니다.`, ...newState.logs].slice(0, 50);
          }
          break;

        default:
          break;
      }
      return newState;
    });
  }, []);

  const unionBonus = getUnionBonus(gameState);

  return { 
    gameState: {
      ...gameState,
      player: gameState.characters[gameState.activeSlot],
      unionBonus
    }, 
    dispatch 
  };
}
