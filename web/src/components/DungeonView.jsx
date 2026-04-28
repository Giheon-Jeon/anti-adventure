import { useState } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { Sword, Skull, ShieldAlert } from 'lucide-react';

export default function DungeonView({ state, dispatch }) {
  const [selectedDungeon, setSelectedDungeon] = useState(null);
  const [inCombat, setInCombat] = useState(false);
  const [monster, setMonster] = useState(null);

  const startCombat = (dungeon) => {
    const monsters = {
      'field': ['슬라임', '주황버섯', '스텀프'],
      'forest': ['와일드보어', '다크스텀프', '루팡'],
      'cave': ['뿔버섯', '좀비버섯', '이블아이']
    };
    
    const possibleMonsters = monsters[dungeon.id] || monsters['field'];
    const randomMonster = possibleMonsters[Math.floor(Math.random() * possibleMonsters.length)];
    
    setMonster({
      name: randomMonster,
      hp: dungeon.recLevel * 30,
      maxHp: dungeon.recLevel * 30,
      attack: dungeon.recLevel * 5
    });
    
    setSelectedDungeon(dungeon);
    setInCombat(true);
    dispatch({ type: 'ADD_LOG', payload: `${dungeon.name}에 진입했습니다. 야생의 ${randomMonster}이(가) 나타났다!` });
  };

  const attack = () => {
    if (!monster) return;

    // Player attacks
    const damage = state.player.stats.str * 2 + Math.floor(Math.random() * 5);
    const newMonsterHp = Math.max(0, monster.hp - damage);
    
    dispatch({ type: 'ADD_LOG', payload: `${monster.name}에게 ${damage}의 피해를 입혔습니다!` });

    if (newMonsterHp === 0) {
      // Monster defeated
      const earnedGold = selectedDungeon.recLevel * 10 + Math.floor(Math.random() * 10);
      const earnedExp = selectedDungeon.recLevel * 20;
      
      dispatch({ type: 'COMBAT_WIN', payload: { gold: earnedGold, exp: earnedExp, monsterName: monster.name } });
      setInCombat(false);
      setMonster(null);
      return;
    }

    // Monster attacks back
    const monsterDamage = Math.max(1, monster.attack - Math.floor(state.player.stats.dex * 0.5));
    // Hacky HP update directly for now, should use dispatch in real app
    state.player.hp -= monsterDamage;
    dispatch({ type: 'ADD_LOG', payload: `${monster.name}의 공격! ${monsterDamage}의 피해를 입었습니다.` });
    
    if (state.player.hp <= 0) {
      state.player.hp = 1; // Don't die completely for demo
      dispatch({ type: 'ADD_LOG', payload: '체력이 0이 되어 눈앞이 깜깜해졌다... 마을로 돌아갑니다.' });
      setInCombat(false);
      setMonster(null);
    } else {
      setMonster({ ...monster, hp: newMonsterHp });
    }
  };

  const runAway = () => {
    dispatch({ type: 'ADD_LOG', payload: '무사히 도망쳤습니다!' });
    setInCombat(false);
    setMonster(null);
  };

  return (
    <div className="dungeon-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px', height: '100%' }}>
      <h1 className="text-gradient" style={{ fontSize: '2rem' }}>던전</h1>
      
      <AnimatePresence mode="wait">
        {!inCombat ? (
          <motion.div 
            key="selection"
            initial={{ opacity: 0, x: -20 }}
            animate={{ opacity: 1, x: 0 }}
            exit={{ opacity: 0, x: 20 }}
            className="card-grid"
          >
            {state.dungeons.map(dungeon => (
              <div key={dungeon.id} className="stat-card glass-panel">
                <div className="stat-header">
                  <h3 style={{ display: 'flex', alignItems: 'center', gap: '8px' }}>
                    <Skull size={18} className={dungeon.recLevel > state.player.level ? 'text-danger' : 'text-text-main'} />
                    {dungeon.name}
                  </h3>
                  <span style={{ fontSize: '0.8rem', color: 'var(--text-muted)' }}>권장 Lv.{dungeon.recLevel}</span>
                </div>
                <button 
                  className={`btn ${dungeon.recLevel > state.player.level ? 'btn-danger' : 'btn-primary'}`}
                  style={{ marginTop: '15px' }}
                  onClick={() => startCombat(dungeon)}
                >
                  <Sword size={16} /> 진입하기
                </button>
              </div>
            ))}
          </motion.div>
        ) : (
          <motion.div
            key="combat"
            initial={{ opacity: 0, scale: 0.9 }}
            animate={{ opacity: 1, scale: 1 }}
            exit={{ opacity: 0, scale: 0.9 }}
            className="combat-arena glass-panel"
            style={{ flex: 1, display: 'flex', flexDirection: 'column', padding: '30px', alignItems: 'center', justifyContent: 'center', gap: '30px' }}
          >
            <div style={{ textAlign: 'center' }}>
              <h2 style={{ fontSize: '2rem', color: 'var(--danger)', marginBottom: '10px' }}>{monster.name}</h2>
              <div style={{ width: '300px', margin: '0 auto' }}>
                <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.9rem', marginBottom: '5px' }}>
                  <span>HP</span>
                  <span>{monster.hp} / {monster.maxHp}</span>
                </div>
                <div className="progress-bar-bg" style={{ height: '12px' }}>
                  <div className="progress-bar-fill hp-fill" style={{ width: `${(monster.hp / monster.maxHp) * 100}%` }} />
                </div>
              </div>
            </div>

            <div style={{ display: 'flex', gap: '20px', marginTop: '20px' }}>
              <button className="btn btn-primary" style={{ padding: '15px 30px', fontSize: '1.2rem' }} onClick={attack}>
                <Sword size={24} /> 공격 (Enter)
              </button>
              <button className="btn" style={{ padding: '15px 30px', fontSize: '1.2rem' }} onClick={runAway}>
                도망치기
              </button>
            </div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}
