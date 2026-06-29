/**
 * DungeonView - 던전 탐험 및 전투 화면
 *
 * 화면 구성:
 * 1. 던전 선택 목록 (비전투 상태)
 * 2. 전투 아레나 (전투 중 상태)
 *
 * 전투 흐름:
 * 1. 몬스터 등장 (startCombat)
 * 2. 플레이어 공격 버튼 클릭 (attack)
 * 3. 몬스터 선공 → 플레이어 반격 순서
 * 4. 몬스터 HP 0 → 보상 지급 (COMBAT_WIN)
 *    또는 플레이어 HP 0 → 전투 종료 (마을 복귀)
 *
 * 도감 마스터(30회 처치) 시 즉사 처리로 전투 생략.
 */
import { useState } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { Sword, Skull, Dices } from 'lucide-react';
import { evaluateDice, calcSkillEffects } from '../lib/combatUtils';
import { getMonsterDrops } from '../data/monsterLoot';
import { pickRandomMonster } from '../data/dungeonMonsters';

// ── 하위 컴포넌트 ─────────────────────────────────────────────────────────

/**
 * 주사위 눈금 배열을 시각적으로 표시하는 컴포넌트.
 * @param {{ diceArr: number[] }} props
 */
function DiceRow({ diceArr }) {
  return (
    <div style={{ display: 'flex', gap: '5px', justifyContent: 'center', flexWrap: 'wrap' }}>
      {diceArr.map((d, i) => (
        <div
          key={i}
          style={{
            width: '30px', height: '30px',
            background: 'rgba(255,255,255,0.1)',
            border: '1px solid var(--border)',
            borderRadius: '6px',
            display: 'flex', alignItems: 'center', justifyContent: 'center',
            fontWeight: 'bold',
          }}
        >
          {d}
        </div>
      ))}
    </div>
  );
}

/**
 * 전투 결과 패널 (내 주사위 / 적 주사위 나란히 표시).
 * @param {{ result: Object }} props
 */
function BattleResultPanel({ result }) {
  if (result.phase === 'rolling') {
    return (
      <motion.div animate={{ rotate: 360 }} transition={{ repeat: Infinity, duration: 0.5, ease: 'linear' }}>
        <Dices size={48} color="var(--primary)" />
      </motion.div>
    );
  }

  return (
    <div style={{ display: 'flex', justifyContent: 'space-around', width: '100%', gap: '10px' }}>
      {/* 플레이어 측 */}
      <motion.div
        initial={{ scale: 0 }} animate={{ scale: 1 }}
        style={{ flex: 1, textAlign: 'center', background: 'rgba(52,211,153,0.1)', padding: '15px 10px', borderRadius: '8px', border: '1px solid rgba(52,211,153,0.3)' }}
      >
        <div style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginBottom: '8px' }}>내 주사위 (5개)</div>
        <DiceRow diceArr={result.playerDiceArr} />
        <div style={{ fontSize: '0.8rem', color: '#34d399', marginTop: '8px', fontWeight: 'bold' }}>{result.playerEval.name}</div>
        <div style={{ fontSize: '0.9rem', marginTop: '5px' }}>총 피해: <strong style={{ color: 'var(--danger)', fontSize: '1.2rem' }}>{result.playerDamage}</strong></div>
      </motion.div>

      {/* 몬스터 측 */}
      <motion.div
        initial={{ scale: 0 }} animate={{ scale: 1 }} transition={{ delay: 0.5 }}
        style={{ flex: 1, textAlign: 'center', background: 'rgba(248,113,113,0.1)', padding: '15px 10px', borderRadius: '8px', border: '1px solid rgba(248,113,113,0.3)' }}
      >
        <div style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginBottom: '8px' }}>적 주사위 (3개)</div>
        <DiceRow diceArr={result.monsterDiceArr} />
        <div style={{ fontSize: '0.8rem', color: '#f87171', marginTop: '8px', fontWeight: 'bold' }}>{result.monsterEval.name}</div>
        <div style={{ fontSize: '0.9rem', marginTop: '5px' }}>반격: <strong style={{ color: 'var(--danger)', fontSize: '1.2rem' }}>{result.monsterDamage}</strong></div>
      </motion.div>
    </div>
  );
}

// ── 메인 컴포넌트 ─────────────────────────────────────────────────────────

/**
 * DungeonView - 최소 권한으로 필요한 데이터만 받습니다.
 *
 * @param {{
 *   dungeons:    Array,   // 던전 목록
 *   encyclopedia: Array,  // 도감 (마스터 즉사 판별용)
 *   player:      Object,  // 현재 플레이어 (스탯·스킬)
 *   unionBonus:  Object,  // 유니온 보너스
 *   dispatch:    Function
 * }} props
 */
export default function DungeonView({ dungeons, encyclopedia, player, unionBonus, dispatch }) {
  const [selectedDungeon, setSelectedDungeon] = useState(null);
  const [inCombat, setInCombat]               = useState(false);
  const [monster, setMonster]                 = useState(null);
  const [isRolling, setIsRolling]             = useState(false);
  const [battleResult, setBattleResult]       = useState(null);

  // ── 전투 시작 ────────────────────────────────────────────────────────────
  const startCombat = (dungeon) => {
    const monsterName = pickRandomMonster(dungeon.id);

    // 도감 마스터(30회 처치) 시 즉사 처리
    const entry = encyclopedia.find(e => e.name === monsterName);
    if (entry && entry.kills >= 30) {
      const earnedGold = dungeon.recLevel * 10 + Math.floor(Math.random() * 10);
      const earnedExp  = dungeon.recLevel * 20;
      const drops      = getMonsterDrops(monsterName);

      dispatch({ type: 'ADD_LOG',    payload: `[도감 마스터] ${monsterName}의 약점을 간파하여 즉시 처치했습니다! (+${earnedGold}G, +${earnedExp}EXP)` });
      dispatch({ type: 'COMBAT_WIN', payload: { gold: earnedGold, exp: earnedExp, monsterName, drops } });

      // 즉사 연출 후 전투 화면 해제
      setSelectedDungeon(dungeon);
      setMonster({ name: monsterName, instantKilled: true, hp: 0, maxHp: dungeon.recLevel * 30 });
      setInCombat(true);
      setTimeout(() => { setInCombat(false); setMonster(null); }, 2000);
      return;
    }

    // 일반 전투 초기화
    setMonster({
      name:   monsterName,
      hp:     dungeon.recLevel * 40,
      maxHp:  dungeon.recLevel * 40,
      attack: dungeon.recLevel * 8,
    });
    setSelectedDungeon(dungeon);
    setInCombat(true);
    setBattleResult(null);
    dispatch({ type: 'ADD_LOG', payload: `${dungeon.name}에 진입했습니다. 야생의 ${monsterName}이(가) 나타났다!` });
  };

  // ── 공격 ─────────────────────────────────────────────────────────────────
  const attack = () => {
    if (!monster || isRolling || monster.instantKilled) return;
    setIsRolling(true);

    // 기본 스킬 레벨 조회
    const powerStrikeLv = player.skills?.find(s => s.id === 'power_strike')?.level || 0;
    const ironSkinLv    = player.skills?.find(s => s.id === 'iron_skin')?.level    || 0;
    const luckyDiceLv   = player.skills?.find(s => s.id === 'lucky_dice')?.level   || 0;
    const ub            = unionBonus || { str: 0, dex: 0 };

    // 직업 스킬 보정값 집계
    const { extraFlatDmg, extraDmgReduc, extraDiceSum, extraMult, lifesteal } = calcSkillEffects(player.skills);

    // 플레이어 주사위 5개 (lucky_dice 레벨만큼 보정)
    const playerDiceArr = Array.from({ length: 5 }, () => Math.floor(Math.random() * 6) + 1 + luckyDiceLv);
    const playerEval    = evaluateDice(playerDiceArr);

    // 최종 플레이어 피해량
    const basePlayerDmg = (player.stats.str + ub.str) * 2
      + playerEval.sum + extraDiceSum
      + (powerStrikeLv * 5) + extraFlatDmg;
    const playerDamage = Math.floor(basePlayerDmg * (playerEval.mult + extraMult));

    // 몬스터 주사위 3개
    const monsterDiceArr = Array.from({ length: 3 }, () => Math.floor(Math.random() * 6) + 1);
    const monsterEval    = evaluateDice(monsterDiceArr);

    // 최종 몬스터 피해량 (DEX로 일부 방어, 아이언스킨·스킬 방어 차감)
    const baseMonsterDmg = monster.attack + monsterEval.sum
      - Math.floor((player.stats.dex + ub.dex) * 0.5);
    const monsterDamage = Math.max(
      1,
      Math.floor(Math.max(1, baseMonsterDmg) * monsterEval.mult) - (ironSkinLv * 3) - extraDmgReduc,
    );

    // 주사위 굴리는 중 애니메이션 표시
    setBattleResult({ playerDiceArr, playerEval, playerDamage, monsterDiceArr, monsterEval, monsterDamage, phase: 'rolling' });

    setTimeout(() => {
      setBattleResult(prev => ({ ...prev, phase: 'result' }));

      // ① 몬스터 선공
      dispatch({ type: 'TAKE_DAMAGE', payload: monsterDamage });
      dispatch({ type: 'ADD_LOG',     payload: `${monster.name}의 [${monsterEval.name}] 선제 공격! ${monsterDamage}의 피해.` });

      // 플레이어 사망 판정 (note: hp 변경은 다음 렌더에 반영되므로 직접 계산)
      if (player.hp - monsterDamage <= 0) {
        dispatch({ type: 'ADD_LOG', payload: '체력이 0이 되어 눈앞이 깜깜해졌다... 마을로 돌아갑니다.' });
        setTimeout(() => { setInCombat(false); setMonster(null); setIsRolling(false); setBattleResult(null); }, 2000);
        return;
      }

      // ② 플레이어 반격
      setTimeout(() => {
        const newMonsterHp = Math.max(0, monster.hp - playerDamage);
        dispatch({ type: 'ADD_LOG', payload: `[${playerEval.name}] 반격! ${monster.name}에게 ${playerDamage}의 피해!` });

        // 흡혈(lifesteal) 처리
        if (lifesteal > 0) {
          const healAmt = Math.floor(playerDamage * lifesteal);
          dispatch({ type: 'TAKE_DAMAGE', payload: -healAmt }); // 음수 payload = 회복
          dispatch({ type: 'ADD_LOG',     payload: `피의 굶주림으로 ${healAmt}만큼 체력을 흡수했다!` });
        }

        // 몬스터 사망 → 보상 지급
        if (newMonsterHp === 0) {
          setTimeout(() => {
            const earnedGold = selectedDungeon.recLevel * 10 + Math.floor(Math.random() * 10);
            const earnedExp  = selectedDungeon.recLevel * 20;
            const drops      = getMonsterDrops(monster.name);
            dispatch({ type: 'COMBAT_WIN', payload: { gold: earnedGold, exp: earnedExp, monsterName: monster.name, drops } });
            setInCombat(false); setMonster(null); setIsRolling(false); setBattleResult(null);
          }, 2000);
        } else {
          // 몬스터 생존 → HP 갱신 후 다음 턴 대기
          setMonster(prev => ({ ...prev, hp: newMonsterHp }));
          setIsRolling(false);
        }
      }, 1500);
    }, 1500);
  };

  // ── 도망 ─────────────────────────────────────────────────────────────────
  const runAway = () => {
    dispatch({ type: 'ADD_LOG', payload: '무사히 도망쳤습니다!' });
    setInCombat(false);
    setMonster(null);
    setBattleResult(null);
  };

  // ── 렌더 ─────────────────────────────────────────────────────────────────
  return (
    <div className="dungeon-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px', height: '100%' }}>
      <h1 className="text-gradient" style={{ fontSize: '2rem' }}>던전</h1>

      <AnimatePresence mode="wait">
        {/* 던전 선택 화면 */}
        {!inCombat ? (
          <motion.div
            key="selection"
            initial={{ opacity: 0, x: -20 }}
            animate={{ opacity: 1, x: 0 }}
            exit={{ opacity: 0, x: 20 }}
            className="card-grid"
          >
            {dungeons.map(dungeon => (
              <div key={dungeon.id} className="stat-card glass-panel">
                <div className="stat-header">
                  <h3 style={{ display: 'flex', alignItems: 'center', gap: '8px' }}>
                    {/* 권장 레벨 미달 시 경고 아이콘 색 변경 */}
                    <Skull size={18} className={dungeon.recLevel > player.level ? 'text-danger' : 'text-text-main'} />
                    {dungeon.name}
                  </h3>
                  <span style={{ fontSize: '0.8rem', color: 'var(--text-muted)' }}>권장 Lv.{dungeon.recLevel}</span>
                </div>
                <button
                  className={`btn ${dungeon.recLevel > player.level ? 'btn-danger' : 'btn-primary'}`}
                  style={{ marginTop: '15px' }}
                  onClick={() => startCombat(dungeon)}
                >
                  <Sword size={16} /> 진입하기
                </button>
              </div>
            ))}
          </motion.div>
        ) : (
          /* 전투 화면 */
          <motion.div
            key="combat"
            initial={{ opacity: 0, scale: 0.9 }}
            animate={{ opacity: 1, scale: 1 }}
            exit={{ opacity: 0, scale: 0.9 }}
            className="combat-arena glass-panel"
            style={{ flex: 1, display: 'flex', flexDirection: 'column', padding: '20px', alignItems: 'center', justifyContent: 'center', gap: '20px', position: 'relative' }}
          >
            {/* 몬스터 HP 표시 */}
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

            {/* 주사위 결과 및 액션 버튼 (즉사 시 숨김) */}
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
                        <BattleResultPanel result={battleResult} />
                      </motion.div>
                    )}
                  </AnimatePresence>
                </div>

                <div style={{ display: 'flex', gap: '20px', marginTop: 'auto' }}>
                  <button
                    className="btn btn-primary"
                    style={{ padding: '15px 30px', fontSize: '1.2rem', opacity: isRolling ? 0.5 : 1 }}
                    onClick={attack}
                    disabled={isRolling}
                  >
                    <Sword size={24} /> {isRolling ? '전투 중...' : '공격 (Enter)'}
                  </button>
                  <button
                    className="btn"
                    style={{ padding: '15px 30px', fontSize: '1.2rem' }}
                    onClick={runAway}
                    disabled={isRolling}
                  >
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
