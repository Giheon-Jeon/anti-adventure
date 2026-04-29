import { useState } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { Sword, Skull, ShieldAlert, Dices } from 'lucide-react';

export default function DungeonView({ state, dispatch }) {
  const [selectedDungeon, setSelectedDungeon] = useState(null);
  const [inCombat, setInCombat] = useState(false);
  const [monster, setMonster] = useState(null);
  
  const [isRolling, setIsRolling] = useState(false);
  const [battleResult, setBattleResult] = useState(null);

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
    setBattleResult(null);
    dispatch({ type: 'ADD_LOG', payload: `${dungeon.name}에 진입했습니다. 야생의 ${randomMonster}이(가) 나타났다!` });
  };

  const attack = () => {
    if (!monster || isRolling) return;
    setIsRolling(true);

    // Get skill levels
    const powerStrikeLv = state.player.skills?.find(s => s.id === 'power_strike')?.level || 0;
    const ironSkinLv = state.player.skills?.find(s => s.id === 'iron_skin')?.level || 0;
    const luckyDiceLv = state.player.skills?.find(s => s.id === 'lucky_dice')?.level || 0;

    const basePlayerDice = Math.floor(Math.random() * 6) + 1;
    const playerDice = basePlayerDice + luckyDiceLv;
    const playerDamage = state.player.stats.str * 2 + playerDice + (powerStrikeLv * 5);

    const monsterDice = Math.floor(Math.random() * 6) + 1;
    let monsterDamage = Math.max(1, monster.attack + monsterDice - Math.floor(state.player.stats.dex * 0.5));
    monsterDamage = Math.max(1, monsterDamage - (ironSkinLv * 3));

    setBattleResult({
      playerDice,
      playerDamage,
      monsterDice,
      monsterDamage,
      phase: 'rolling'
    });

    // Roll animation delay
    setTimeout(() => {
      setBattleResult(prev => ({ ...prev, phase: 'result' }));
      
      const newMonsterHp = Math.max(0, monster.hp - playerDamage);
      dispatch({ type: 'ADD_LOG', payload: `주사위[${playerDice}]! ${monster.name}에게 ${playerDamage}의 피해를 입혔습니다!` });

      if (newMonsterHp === 0) {
        // Monster defeated
        setTimeout(() => {
          const earnedGold = selectedDungeon.recLevel * 10 + Math.floor(Math.random() * 10);
          const earnedExp = selectedDungeon.recLevel * 20;
          
          dispatch({ type: 'COMBAT_WIN', payload: { gold: earnedGold, exp: earnedExp, monsterName: monster.name } });
          setInCombat(false);
          setMonster(null);
          setIsRolling(false);
          setBattleResult(null);
        }, 1500);
        return;
      }

      // Monster attacks back
      setTimeout(() => {
        dispatch({ type: 'TAKE_DAMAGE', payload: monsterDamage });
        dispatch({ type: 'ADD_LOG', payload: `${monster.name}의 주사위[${monsterDice}] 반격! ${monsterDamage}의 피해를 입었습니다.` });
        
        if (state.player.hp - monsterDamage <= 0) {
          dispatch({ type: 'ADD_LOG', payload: '체력이 0이 되어 눈앞이 깜깜해졌다... 마을로 돌아갑니다.' });
          setTimeout(() => {
            setInCombat(false);
            setMonster(null);
            setIsRolling(false);
            setBattleResult(null);
          }, 1500);
        } else {
          setMonster(prev => ({ ...prev, hp: newMonsterHp }));
          setIsRolling(false);
        }
      }, 1000);
    }, 1000);
  };

  const runAway = () => {
    dispatch({ type: 'ADD_LOG', payload: '무사히 도망쳤습니다!' });
    setInCombat(false);
    setMonster(null);
    setBattleResult(null);
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
            style={{ flex: 1, display: 'flex', flexDirection: 'column', padding: '20px', alignItems: 'center', justifyContent: 'center', gap: '20px', position: 'relative' }}
          >
            <div style={{ textAlign: 'center', width: '100%' }}>
              <h2 style={{ fontSize: '2rem', color: 'var(--danger)', marginBottom: '10px' }}>{monster.name}</h2>
              <div style={{ width: '100%', maxWidth: '300px', margin: '0 auto' }}>
                <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.9rem', marginBottom: '5px' }}>
                  <span>HP</span>
                  <span>{monster.hp} / {monster.maxHp}</span>
                </div>
                <div className="progress-bar-bg" style={{ height: '12px' }}>
                  <div className="progress-bar-fill hp-fill" style={{ width: `${(monster.hp / monster.maxHp) * 100}%` }} />
                </div>
              </div>
            </div>

            {/* Battle Visuals Area */}
            <div style={{ height: '150px', display: 'flex', alignItems: 'center', justifyContent: 'center', width: '100%' }}>
              <AnimatePresence>
                {battleResult && (
                  <motion.div 
                    initial={{ opacity: 0, y: 20 }}
                    animate={{ opacity: 1, y: 0 }}
                    exit={{ opacity: 0 }}
                    style={{ display: 'flex', flexDirection: 'column', gap: '15px', alignItems: 'center', width: '100%' }}
                  >
                    {battleResult.phase === 'rolling' ? (
                      <motion.div
                        animate={{ rotate: 360 }}
                        transition={{ repeat: Infinity, duration: 0.5, ease: "linear" }}
                      >
                        <Dices size={48} color="var(--primary)" />
                      </motion.div>
                    ) : (
                      <div style={{ display: 'flex', justifyContent: 'space-around', width: '100%', maxWidth: '400px' }}>
                        {/* Player side */}
                        <motion.div initial={{ scale: 0 }} animate={{ scale: 1 }} style={{ textAlign: 'center', background: 'rgba(52,211,153,0.1)', padding: '10px', borderRadius: '8px', border: '1px solid rgba(52,211,153,0.3)' }}>
                          <div style={{ fontSize: '0.8rem', color: 'var(--text-muted)' }}>내 주사위</div>
                          <div style={{ fontSize: '2rem', fontWeight: 'bold', color: '#34d399' }}>🎲 {battleResult.playerDice}</div>
                          <div style={{ fontSize: '0.9rem', marginTop: '5px' }}>피해: <strong style={{ color: 'var(--danger)' }}>{battleResult.playerDamage}</strong></div>
                        </motion.div>
                        
                        {/* Monster side */}
                        <motion.div initial={{ scale: 0 }} animate={{ scale: 1 }} transition={{ delay: 0.5 }} style={{ textAlign: 'center', background: 'rgba(248,113,113,0.1)', padding: '10px', borderRadius: '8px', border: '1px solid rgba(248,113,113,0.3)' }}>
                          <div style={{ fontSize: '0.8rem', color: 'var(--text-muted)' }}>적 주사위</div>
                          <div style={{ fontSize: '2rem', fontWeight: 'bold', color: '#f87171' }}>🎲 {battleResult.monsterDice}</div>
                          <div style={{ fontSize: '0.9rem', marginTop: '5px' }}>반격: <strong style={{ color: 'var(--danger)' }}>{battleResult.monsterDamage}</strong></div>
                        </motion.div>
                      </div>
                    )}
                  </motion.div>
                )}
              </AnimatePresence>
            </div>

            <div style={{ display: 'flex', gap: '20px', marginTop: 'auto' }}>
              <button className="btn btn-primary" style={{ padding: '15px 30px', fontSize: '1.2rem', opacity: isRolling ? 0.5 : 1 }} onClick={attack} disabled={isRolling}>
                <Sword size={24} /> {isRolling ? '전투 중...' : '공격 (Enter)'}
              </button>
              <button className="btn" style={{ padding: '15px 30px', fontSize: '1.2rem' }} onClick={runAway} disabled={isRolling}>
                도망치기
              </button>
            </div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}
