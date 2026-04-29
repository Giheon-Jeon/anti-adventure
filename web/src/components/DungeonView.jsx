import { useState } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { Sword, Skull, Dices } from 'lucide-react';

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
    
    const entry = state.encyclopedia.find(e => e.name === randomMonster);
    if (entry && entry.kills >= 30) {
      const earnedGold = dungeon.recLevel * 10 + Math.floor(Math.random() * 10);
      const earnedExp = dungeon.recLevel * 20;
      dispatch({ type: 'ADD_LOG', payload: `[도감 마스터] ${randomMonster}의 약점을 간파하여 즉시 처치했습니다! (+${earnedGold}G, +${earnedExp}EXP)` });
      dispatch({ type: 'COMBAT_WIN', payload: { gold: earnedGold, exp: earnedExp, monsterName: randomMonster } });
      
      setSelectedDungeon(dungeon);
      setMonster({ name: randomMonster, instantKilled: true, hp: 0, maxHp: dungeon.recLevel * 30 });
      setInCombat(true);
      
      setTimeout(() => {
        setInCombat(false);
        setMonster(null);
      }, 2000);
      return;
    }

    setMonster({
      name: randomMonster,
      hp: dungeon.recLevel * 40, // Increased HP a bit for balance
      maxHp: dungeon.recLevel * 40,
      attack: dungeon.recLevel * 8 // Increased enemy base damage
    });
    
    setSelectedDungeon(dungeon);
    setInCombat(true);
    setBattleResult(null);
    dispatch({ type: 'ADD_LOG', payload: `${dungeon.name}에 진입했습니다. 야생의 ${randomMonster}이(가) 나타났다!` });
  };

  const evaluateDice = (diceArray) => {
    const counts = {};
    let sum = 0;
    diceArray.forEach(d => {
      counts[d] = (counts[d] || 0) + 1;
      sum += d;
    });
    
    const values = Object.values(counts).sort((a, b) => b - a);
    const uniqueFaces = Object.keys(counts).map(Number).sort((a, b) => a - b);
    
    let isStraight = false;
    if (diceArray.length === 5 && uniqueFaces.length === 5) {
      if (uniqueFaces[4] - uniqueFaces[0] === 4) isStraight = true;
    } else if (diceArray.length === 3 && uniqueFaces.length === 3) {
      if (uniqueFaces[2] - uniqueFaces[0] === 2) isStraight = true;
    }

    if (diceArray.length === 5) {
      if (values[0] === 5) return { name: '5 of a Kind!', mult: 5.0, sum };
      if (isStraight) return { name: 'Straight!', mult: 2.5, sum };
      if (values[0] === 4) return { name: '4 of a Kind!', mult: 3.0, sum };
      if (values[0] === 3 && values[1] === 2) return { name: 'Full House!', mult: 2.5, sum };
      if (values[0] === 3) return { name: '3 of a Kind!', mult: 2.0, sum };
      if (values[0] === 2 && values[1] === 2) return { name: 'Two Pairs!', mult: 1.5, sum };
      if (values[0] === 2) return { name: '1 Pair!', mult: 1.2, sum };
      return { name: 'No Combo', mult: 1.0, sum };
    } else {
      if (values[0] === 3) return { name: '3 of a Kind!', mult: 3.0, sum };
      if (isStraight) return { name: 'Straight!', mult: 2.0, sum };
      if (values[0] === 2) return { name: '1 Pair!', mult: 1.5, sum };
      return { name: 'No Combo', mult: 1.0, sum };
    }
  };

  const attack = () => {
    if (!monster || isRolling || monster.instantKilled) return;
    setIsRolling(true);

    const powerStrikeLv = state.player.skills?.find(s => s.id === 'power_strike')?.level || 0;
    const ironSkinLv = state.player.skills?.find(s => s.id === 'iron_skin')?.level || 0;
    const luckyDiceLv = state.player.skills?.find(s => s.id === 'lucky_dice')?.level || 0;
    const unionBonus = state.unionBonus || { str: 0, dex: 0, int: 0, luk: 0 };

    let extraFlatDmg = 0, extraDmgReduc = 0, extraDiceSum = 0, extraMult = 0, lifesteal = 0;
    state.player.skills?.forEach(s => {
      if (s.level === 0) return;
      if (s.id === 'hero_t1') extraFlatDmg += 20 * s.level;
      if (s.id === 'hero_t2') extraDmgReduc += 15 * s.level;
      if (s.id === 'hero_ult') extraMult += 1.0 * s.level;
      if (s.id === 'gambler_t1') extraFlatDmg += 15 * s.level;
      if (s.id === 'gambler_t2') extraMult += 0.5 * s.level;
      if (s.id === 'gambler_ult') { extraMult += 2.0 * s.level; extraDmgReduc -= 50; }
      if (s.id === 'berserk_t1') extraFlatDmg += 25 * s.level;
      if (s.id === 'berserk_t2') lifesteal += 0.1 * s.level;
      if (s.id === 'berserk_ult') extraDmgReduc += 30 * s.level;
      if (s.id === 'thief_t1') extraFlatDmg += 15 * s.level;
      if (s.id === 'thief_t2') extraDmgReduc += 10 * s.level;
      if (s.id === 'thief_ult') extraMult += 0.5 * s.level;
      if (s.id === 'elem_t1') extraFlatDmg += 18 * s.level;
      if (s.id === 'elem_t2') extraDmgReduc += 12 * s.level;
      if (s.id === 'elem_ult') extraMult += 1.0 * s.level;
      if (s.id === 'warrior_t1') extraFlatDmg += 10 * s.level;
      if (s.id === 'warrior_t2') extraDiceSum += 5 * s.level;
      if (s.id === 'warrior_ult') extraMult += 0.5 * s.level;
      if (s.id === 'rogue_t1') extraFlatDmg += 12 * s.level;
      if (s.id === 'rogue_t2') extraDmgReduc += 8 * s.level;
      if (s.id === 'rogue_ult') extraMult += 0.5 * s.level;
      if (s.id === 'mage_t1') extraFlatDmg += 15 * s.level;
      if (s.id === 'mage_t2') extraDmgReduc += 10 * s.level;
      if (s.id === 'mage_ult') extraFlatDmg += 50 * s.level;
      if (s.id === 'jobless_t1') extraFlatDmg += 5 * s.level;
      if (s.id === 'jobless_t2') extraDmgReduc += 5 * s.level;
      if (s.id === 'jobless_ult') extraDiceSum += 10 * s.level;
    });

    // Player 5 Dice
    const playerDiceArr = Array.from({ length: 5 }, () => Math.floor(Math.random() * 6) + 1 + luckyDiceLv);
    const playerEval = evaluateDice(playerDiceArr);
    
    const basePlayerDmg = (state.player.stats.str + unionBonus.str) * 2 + playerEval.sum + extraDiceSum + (powerStrikeLv * 5) + extraFlatDmg;
    const playerDamage = Math.floor(basePlayerDmg * (playerEval.mult + extraMult));

    // Monster 3 Dice
    const monsterDiceArr = Array.from({ length: 3 }, () => Math.floor(Math.random() * 6) + 1);
    const monsterEval = evaluateDice(monsterDiceArr);

    const baseMonsterDmg = monster.attack + monsterEval.sum - Math.floor((state.player.stats.dex + unionBonus.dex) * 0.5);
    let monsterDamage = Math.floor(Math.max(1, baseMonsterDmg) * monsterEval.mult);
    monsterDamage = Math.max(1, monsterDamage - (ironSkinLv * 3) - extraDmgReduc);

    setBattleResult({
      playerDiceArr,
      playerEval,
      playerDamage,
      monsterDiceArr,
      monsterEval,
      monsterDamage,
      phase: 'rolling'
    });

    setTimeout(() => {
      setBattleResult(prev => ({ ...prev, phase: 'result' }));
      
      // Monster attacks first
      dispatch({ type: 'TAKE_DAMAGE', payload: monsterDamage });
      dispatch({ type: 'ADD_LOG', payload: `${monster.name}의 [${monsterEval.name}] 선제 공격! ${monsterDamage}의 피해.` });
      
      if (state.player.hp - monsterDamage <= 0) {
        dispatch({ type: 'ADD_LOG', payload: '체력이 0이 되어 눈앞이 깜깜해졌다... 마을로 돌아갑니다.' });
        setTimeout(() => {
          setInCombat(false);
          setMonster(null);
          setIsRolling(false);
          setBattleResult(null);
        }, 2000);
        return; // Player died
      }

      // Player attacks if survived
      setTimeout(() => {
        const newMonsterHp = Math.max(0, monster.hp - playerDamage);
        dispatch({ type: 'ADD_LOG', payload: `[${playerEval.name}] 반격! ${monster.name}에게 ${playerDamage}의 피해!` });

        if (lifesteal > 0) {
           const healAmt = Math.floor(playerDamage * lifesteal);
           dispatch({ type: 'TAKE_DAMAGE', payload: -healAmt });
           dispatch({ type: 'ADD_LOG', payload: `피의 굶주림으로 ${healAmt}만큼 체력을 흡수했다!` });
        }

        if (newMonsterHp === 0) {
          setTimeout(() => {
            const earnedGold = selectedDungeon.recLevel * 10 + Math.floor(Math.random() * 10);
            const earnedExp = selectedDungeon.recLevel * 20;
            
            dispatch({ type: 'COMBAT_WIN', payload: { gold: earnedGold, exp: earnedExp, monsterName: monster.name } });
            setInCombat(false);
            setMonster(null);
            setIsRolling(false);
            setBattleResult(null);
          }, 2000);
        } else {
          setMonster(prev => ({ ...prev, hp: newMonsterHp }));
          setIsRolling(false);
        }
      }, 1500);
    }, 1500);
  };

  const runAway = () => {
    dispatch({ type: 'ADD_LOG', payload: '무사히 도망쳤습니다!' });
    setInCombat(false);
    setMonster(null);
    setBattleResult(null);
  };

  const renderDice = (diceArr) => {
    return (
      <div style={{ display: 'flex', gap: '5px', justifyContent: 'center', flexWrap: 'wrap' }}>
        {diceArr.map((d, i) => (
          <div key={i} style={{ width: '30px', height: '30px', background: 'rgba(255,255,255,0.1)', border: '1px solid var(--border)', borderRadius: '6px', display: 'flex', alignItems: 'center', justifyContent: 'center', fontWeight: 'bold' }}>
            {d}
          </div>
        ))}
      </div>
    );
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
              {monster.instantKilled ? (
                <motion.div initial={{ scale: 0 }} animate={{ scale: 1 }} style={{ color: '#34d399', fontSize: '1.5rem', fontWeight: 'bold' }}>
                  ⚡ 약점 간파! 즉시 처치! ⚡
                </motion.div>
              ) : (
                <div style={{ width: '100%', maxWidth: '300px', margin: '0 auto' }}>
                  <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.9rem', marginBottom: '5px' }}>
                    <span>HP</span>
                    <span>{monster.hp} / {monster.maxHp}</span>
                  </div>
                  <div className="progress-bar-bg" style={{ height: '12px' }}>
                    <div className="progress-bar-fill hp-fill" style={{ width: `${(monster.hp / monster.maxHp) * 100}%` }} />
                  </div>
                </div>
              )}
            </div>

            {!monster.instantKilled && (
              <>
                <div style={{ height: '220px', display: 'flex', alignItems: 'center', justifyContent: 'center', width: '100%' }}>
                  <AnimatePresence>
                    {battleResult && (
                      <motion.div 
                        initial={{ opacity: 0, y: 20 }}
                        animate={{ opacity: 1, y: 0 }}
                        exit={{ opacity: 0 }}
                        style={{ display: 'flex', flexDirection: 'column', gap: '15px', alignItems: 'center', width: '100%' }}
                      >
                        {battleResult.phase === 'rolling' ? (
                          <motion.div animate={{ rotate: 360 }} transition={{ repeat: Infinity, duration: 0.5, ease: "linear" }}>
                            <Dices size={48} color="var(--primary)" />
                          </motion.div>
                        ) : (
                          <div style={{ display: 'flex', justifyContent: 'space-around', width: '100%', gap: '10px' }}>
                            {/* Player side */}
                            <motion.div initial={{ scale: 0 }} animate={{ scale: 1 }} style={{ flex: 1, textAlign: 'center', background: 'rgba(52,211,153,0.1)', padding: '15px 10px', borderRadius: '8px', border: '1px solid rgba(52,211,153,0.3)' }}>
                              <div style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginBottom: '8px' }}>내 주사위 (5개)</div>
                              {renderDice(battleResult.playerDiceArr)}
                              <div style={{ fontSize: '0.8rem', color: '#34d399', marginTop: '8px', fontWeight: 'bold' }}>{battleResult.playerEval.name}</div>
                              <div style={{ fontSize: '0.9rem', marginTop: '5px' }}>총 피해: <strong style={{ color: 'var(--danger)', fontSize: '1.2rem' }}>{battleResult.playerDamage}</strong></div>
                            </motion.div>
                            
                            {/* Monster side */}
                            <motion.div initial={{ scale: 0 }} animate={{ scale: 1 }} transition={{ delay: 0.5 }} style={{ flex: 1, textAlign: 'center', background: 'rgba(248,113,113,0.1)', padding: '15px 10px', borderRadius: '8px', border: '1px solid rgba(248,113,113,0.3)' }}>
                              <div style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginBottom: '8px' }}>적 주사위 (3개)</div>
                              {renderDice(battleResult.monsterDiceArr)}
                              <div style={{ fontSize: '0.8rem', color: '#f87171', marginTop: '8px', fontWeight: 'bold' }}>{battleResult.monsterEval.name}</div>
                              <div style={{ fontSize: '0.9rem', marginTop: '5px' }}>반격: <strong style={{ color: 'var(--danger)', fontSize: '1.2rem' }}>{battleResult.monsterDamage}</strong></div>
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
              </>
            )}
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}
