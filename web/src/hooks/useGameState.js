import { useState, useCallback, useEffect } from 'react';

const initialState = {
  player: {
    name: 'Adventurer',
    level: 1,
    job: 'Novice',
    hp: 100,
    maxHp: 100,
    mp: 50,
    maxMp: 50,
    exp: 0,
    maxExp: 100,
    gold: 500,
    weapon_tier: 0,
    armor_tier: 0,
    accessory_tier: 0,
    stats: {
      str: 10,
      dex: 10,
      int: 10,
      luk: 10,
    },
    skill_points: 0,
    skills: [
      { id: 'power_strike', name: '파워 스트라이크', level: 0, maxLevel: 5, desc: '적에게 가하는 최종 피해량이 레벨당 5 증가합니다.' },
      { id: 'iron_skin', name: '아이언 스킨', level: 0, maxLevel: 5, desc: '적에게 받는 최종 피해량이 레벨당 3 감소합니다.' },
      { id: 'lucky_dice', name: '행운의 주사위', level: 0, maxLevel: 5, desc: '내 주사위 눈금이 레벨당 1씩 추가로 보정됩니다.' }
    ],
    inventory: [],
  },
  logs: ['Welcome to Anti-Adventure Web!'],
  encyclopedia: [],
  dungeons: [
    { id: 'field', name: '초심자의 들판', recLevel: 1, type: 'easy' },
    { id: 'forest', name: '어두운 숲', recLevel: 5, type: 'normal' },
    { id: 'cave', name: '고블린 동굴', recLevel: 10, type: 'hard' },
  ],
};

export function useGameState() {
  const [gameState, setGameState] = useState(() => {
    const saved = localStorage.getItem('anti-adventure-save');
    if (saved) {
      try { 
        const parsed = JSON.parse(saved);
        // Ensure skills exist for old saves
        if (!parsed.player.skills) parsed.player.skills = initialState.player.skills;
        if (parsed.player.skill_points === undefined) parsed.player.skill_points = 0;
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
      
      switch (action.type) {
        
        case 'WORK_JOB':
          if (newState.player.hp > 15) {
            const roll = Math.floor(Math.random() * 100);
            let gold = 0, hpLoss = 0, msg = '';
            if (roll < 10) { gold = 40; hpLoss = -10; msg = '✨ [대성공!] 사람들에게 칭찬을 들었습니다! (+40G, HP회복 10)'; }
            else if (roll < 60) { gold = 25; hpLoss = 5; msg = '✅ [성공] 무난하게 청소를 마쳤습니다. (+25G, HP감소 5)'; }
            else if (roll < 90) { gold = 10; hpLoss = 15; msg = '❓ [실패] 청소가 덜 된 곳이 있습니다. (+10G, HP감소 15)'; }
            else { gold = 0; hpLoss = 25; msg = '💀 [대실패] 허리를 삐끗했습니다! (+0G, HP감소 25)'; }
            
            newState.player.gold += gold;
            newState.player.hp -= hpLoss;
            if (newState.player.hp > newState.player.maxHp) newState.player.hp = newState.player.maxHp;
            if (newState.player.hp < 1) newState.player.hp = 1;
            newState.logs = [msg, ...newState.logs].slice(0, 50);
          } else {
            newState.logs = ['체력이 부족하여 알바를 할 수 없습니다. (HP 15 초과 필요)', ...newState.logs].slice(0, 50);
          }
          break;

        case 'BUY_ITEM':
          const { cost, statIncreases, newTier, type } = action.payload;
          if (newState.player.gold >= cost) {
            newState.player.gold -= cost;
            if (statIncreases.str) newState.player.stats.str += statIncreases.str;
            if (statIncreases.dex) newState.player.stats.dex += statIncreases.dex;
            if (statIncreases.int) newState.player.stats.int += statIncreases.int;
            if (statIncreases.luk) newState.player.stats.luk += statIncreases.luk;
            if (statIncreases.maxHp) { newState.player.maxHp += statIncreases.maxHp; newState.player.hp = newState.player.maxHp; }
            if (statIncreases.maxMp) { newState.player.maxMp += statIncreases.maxMp; newState.player.mp = newState.player.maxMp; }
            if (type === 'weapon') newState.player.weapon_tier = newTier;
            if (type === 'armor') newState.player.armor_tier = newTier;
            if (type === 'accessory') newState.player.accessory_tier = newTier;
            newState.logs = [`상점에서 장비를 구매했습니다! (-${cost}G)`, ...newState.logs].slice(0, 50);
          } else {
            newState.logs = [`장비를 구매할 골드가 부족합니다! (${cost}G 필요)`, ...newState.logs].slice(0, 50);
          }
          break;

        case 'BUY_POTION':
          if (newState.player.gold >= 30) {
            newState.player.gold -= 30;
            if (action.payload === 'hp') {
              newState.player.hp = Math.min(newState.player.maxHp, newState.player.hp + Math.floor(newState.player.maxHp / 2));
              newState.logs = ['빨간 포션을 마셔 체력을 회복했습니다.', ...newState.logs].slice(0, 50);
            } else {
              newState.player.mp = Math.min(newState.player.maxMp, newState.player.mp + Math.floor(newState.player.maxMp / 2));
              newState.logs = ['파란 포션을 마셔 마나를 회복했습니다.', ...newState.logs].slice(0, 50);
            }
          } else {
            newState.logs = ['포션을 살 골드가 부족합니다! (30G 필요)', ...newState.logs].slice(0, 50);
          }
          break;
          
        case 'REPAIR_ALL':
          if (!newState.player.needsRepair) {
             newState.logs = ['수리할 장비가 없습니다.', ...newState.logs].slice(0, 50);
             break;
          }
          if (newState.player.gold >= action.payload.cost) {
            newState.player.gold -= action.payload.cost;
            newState.player.needsRepair = false;
            newState.logs = [`모든 장비가 완벽하게 수리되었습니다! (-${action.payload.cost}G)`, ...newState.logs].slice(0, 50);
          } else {
            newState.logs = ['수리비가 부족합니다!', ...newState.logs].slice(0, 50);
          }
          break;

        case 'ADD_LOG':
          newState.logs = [action.payload, ...newState.logs].slice(0, 50);
          break;
          
        case 'EARN_GOLD':
          newState.player.gold += action.payload;
          newState.logs = [`+${action.payload} Gold 획득!`, ...newState.logs].slice(0, 50);
          break;
          
        case 'SPEND_GOLD':
          if (newState.player.gold >= action.payload) {
            newState.player.gold -= action.payload;
            return newState;
          }
          return prev;

        case 'HEAL':
          newState.player.hp = newState.player.maxHp;
          newState.player.mp = newState.player.maxMp;
          newState.logs = ['체력과 마나가 모두 회복되었습니다.', ...newState.logs].slice(0, 50);
          break;

        case 'GAIN_EXP':
          newState.player.exp += action.payload;
          let levelUp = false;
          while (newState.player.exp >= newState.player.maxExp) {
            newState.player.level += 1;
            newState.player.exp -= newState.player.maxExp;
            newState.player.maxExp = Math.floor(newState.player.maxExp * 1.5);
            newState.player.maxHp += 20;
            newState.player.hp = newState.player.maxHp;
            newState.player.stats.str += 2;
            newState.player.stats.dex += 2;
            newState.player.stats.int += 2;
            newState.player.stats.luk += 2;
            newState.player.skill_points += 1;
            levelUp = true;
          }
          if (levelUp) {
            newState.logs = [`레벨업! 현재 레벨: ${newState.player.level} (+스킬포인트 1)`, ...newState.logs].slice(0, 50);
          }
          break;
          
        case 'COMBAT_WIN':
          const { gold, exp, monsterName } = action.payload;
          newState.player.gold += gold;
          newState.player.exp += exp;
          newState.logs = [`${monsterName} 처치! +${gold}G, +${exp}EXP`, ...newState.logs].slice(0, 50);
          
          let levelUpBattle = false;
          while (newState.player.exp >= newState.player.maxExp) {
            newState.player.level += 1;
            newState.player.exp -= newState.player.maxExp;
            newState.player.maxExp = Math.floor(newState.player.maxExp * 1.5);
            newState.player.maxHp += 20;
            newState.player.hp = newState.player.maxHp;
            newState.player.stats.str += 2;
            newState.player.stats.dex += 2;
            newState.player.stats.int += 2;
            newState.player.stats.luk += 2;
            newState.player.skill_points += 1;
            levelUpBattle = true;
          }
          if (levelUpBattle) {
            newState.logs = [`레벨업! 현재 레벨: ${newState.player.level} (+스킬포인트 1)`, ...newState.logs].slice(0, 50);
          }
          
          // Encyclopedia update
          const entry = newState.encyclopedia.find(e => e.name === monsterName);
          if (entry) {
            entry.kills += 1;
          } else {
            newState.encyclopedia.push({ name: monsterName, kills: 1 });
          }
          break;
          
        case 'TRAIN':
          const stat = action.payload;
          newState.player.stats[stat] += 1;
          newState.logs = [`훈련을 통해 ${stat.toUpperCase()}이(가) 1 상승했습니다. (-50G)`, ...newState.logs].slice(0, 50);
          break;

        case 'TAKE_DAMAGE':
          newState.player.hp -= action.payload;
          if (newState.player.hp <= 0) {
            newState.player.hp = 1; // Don't die completely for demo
          }
          if (Math.random() < 0.2) {
            newState.player.needsRepair = true;
          }
          break;
          
        case 'UPGRADE_SKILL':
          const skillId = action.payload;
          if (newState.player.skill_points > 0) {
            const skill = newState.player.skills.find(s => s.id === skillId);
            if (skill && skill.level < skill.maxLevel) {
              skill.level += 1;
              newState.player.skill_points -= 1;
              newState.logs = [`${skill.name} 스킬이 레벨업 되었습니다! (Lv.${skill.level})`, ...newState.logs].slice(0, 50);
            } else {
              newState.logs = ['이미 마스터한 스킬이거나 스킬을 찾을 수 없습니다.', ...newState.logs].slice(0, 50);
            }
          } else {
            newState.logs = ['스킬 포인트가 부족합니다.', ...newState.logs].slice(0, 50);
          }
          break;

        default:
          break;
      }
      return newState;
    });
  }, []);

  return { gameState, dispatch };
}
