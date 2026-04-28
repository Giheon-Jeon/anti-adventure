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
    stats: {
      str: 10,
      dex: 10,
      int: 10,
      luk: 10,
    },
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
      try { return JSON.parse(saved); } catch (e) { return initialState; }
    }
    return initialState;
  });

  useEffect(() => {
    localStorage.setItem('anti-adventure-save', JSON.stringify(gameState));
  }, [gameState]);

  const dispatch = useCallback((action) => {
    setGameState((prev) => {
      let newState = { ...prev };
      
      switch (action.type) {
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
            levelUp = true;
          }
          if (levelUp) {
            newState.logs = [`레벨업! 현재 레벨: ${newState.player.level}`, ...newState.logs].slice(0, 50);
          }
          break;
          
        case 'COMBAT_WIN':
          const { gold, exp, monsterName } = action.payload;
          newState.player.gold += gold;
          newState.player.exp += exp;
          newState.logs = [`${monsterName} 처치! +${gold}G, +${exp}EXP`, ...newState.logs].slice(0, 50);
          
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
          if (newState.player.hp > 10) {
            newState.player.hp -= 10;
            newState.player.stats[stat] += 1;
            newState.logs = [`훈련을 통해 ${stat.toUpperCase()}이(가) 1 상승했습니다. (HP -10)`, ...newState.logs].slice(0, 50);
          } else {
            newState.logs = ['체력이 부족하여 훈련할 수 없습니다.', ...newState.logs].slice(0, 50);
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
