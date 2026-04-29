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
  needsRepair: false
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
  ],
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
      if (char.job === 'Novice') {
         bonus.maxHp += 10; // Extra HP for novice
      }
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
          parsed.characters = [oldPlayer, null, null];
          parsed.activeSlot = 0;
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
            newState.logs = [msg, ...newState.logs].slice(0, 50);
          } else {
            newState.logs = ['체력이 부족하여 알바를 할 수 없습니다. (HP 15 초과 필요)', ...newState.logs].slice(0, 50);
          }
          break;

        case 'BUY_ITEM':
          const { cost, statIncreases, newTier, type } = action.payload;
          if (player.gold >= cost) {
            player.gold -= cost;
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
          }
          break;
          
        case 'COMBAT_WIN':
          const { gold, exp, monsterName } = action.payload;
          player.gold += gold;
          player.exp += exp;
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
          newState.logs = [`훈련을 통해 ${stat.toUpperCase()}이(가) 1 상승했습니다. (-50G)`, ...newState.logs].slice(0, 50);
          break;

        case 'TAKE_DAMAGE':
          player.hp -= action.payload;
          if (player.hp <= 0) {
            player.hp = 1; 
          }
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
