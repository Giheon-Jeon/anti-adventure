/**
 * VillageView - 마을 허브 화면
 *
 * 마을에서 할 수 있는 활동:
 * - 훈련소: 50G로 무작위 스탯 +1 (60초 쿨다운)
 * - 여관:   50G로 HP·MP 전체 회복
 * - 마을 알바: 골드 획득 (5회 후 60초 쿨다운)
 * - 상점:   장비·포션 구매 (ShopView로 이동)
 * - 대장간: 장비 수리·제작 (BlacksmithView로 이동)
 * - 정보 상인: 500G로 전직 힌트 획득
 *
 * 최소 권한 props:
 * - gold, hp, maxHp, mp, maxMp: 알바/훈련 조건 및 버튼 색상 표시
 * - dispatch, setActiveTab
 */
import { useState, useEffect } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { Dumbbell, HeartPulse, Coins, Hammer, ShoppingCart, ArrowUpCircle, BookOpen } from 'lucide-react';

/** 알바 1회당 쿨다운 없이 연속 가능한 최대 횟수 */
const MAX_JOB_STREAK = 5;
/** 연속 횟수 초과 또는 훈련 후 쿨다운 (초) */
const COOLDOWN_SECS = 60;

/**
 * @param {{
 *   gold: number,
 *   hp: number, maxHp: number,
 *   mp: number, maxMp: number,
 *   dispatch: Function,
 *   setActiveTab: Function
 * }} props
 */
export default function VillageView({ gold, hp, maxHp, mp, maxMp, dispatch, setActiveTab }) {
  const [isWorking, setIsWorking]     = useState(false);
  const [jobCount, setJobCount]       = useState(0);
  const [jobCooldown, setJobCooldown] = useState(0);
  const [trainCooldown, setTrainCooldown] = useState(0);
  const [trainResult, setTrainResult]     = useState(null); // 훈련 결과 팝업용

  // 1초마다 쿨다운 카운트다운
  useEffect(() => {
    const timer = setInterval(() => {
      setJobCooldown(prev => Math.max(0, prev - 1));
      setTrainCooldown(prev => Math.max(0, prev - 1));
    }, 1000);
    return () => clearInterval(timer);
  }, []);

  /** 훈련소: 50G 소비 후 무작위 스탯 +1 */
  const handleTrain = () => {
    if (trainCooldown > 0) return;
    if (gold < 50) {
      dispatch({ type: 'ADD_LOG', payload: '훈련 비용이 부족합니다! (50G 필요)' });
      return;
    }

    const stats      = ['str', 'dex', 'int', 'luk'];
    const randomStat = stats[Math.floor(Math.random() * stats.length)];

    dispatch({ type: 'SPEND_GOLD', payload: 50 });
    dispatch({ type: 'TRAIN',      payload: randomStat });

    setTrainResult(randomStat.toUpperCase());
    setTrainCooldown(COOLDOWN_SECS);
    setTimeout(() => setTrainResult(null), 3000);
  };

  /** 여관: 50G 소비 후 HP·MP 전체 회복 */
  const handleHeal = () => {
    if (hp === maxHp && mp === maxMp) {
      dispatch({ type: 'ADD_LOG', payload: '이미 체력과 마나가 가득 찼습니다.' });
      return;
    }
    if (gold >= 50) {
      dispatch({ type: 'SPEND_GOLD', payload: 50 });
      dispatch({ type: 'HEAL' });
    } else {
      dispatch({ type: 'ADD_LOG', payload: '여관에 머물 골드가 부족합니다! (50G 필요)' });
    }
  };

  /** 마을 알바: 5회 연속 후 60초 쿨다운 */
  const workJob = () => {
    if (jobCooldown > 0 || isWorking) return;
    setIsWorking(true);
    dispatch({ type: 'WORK_JOB' });

    setJobCount(prev => {
      const next = prev + 1;
      if (next >= MAX_JOB_STREAK) { setJobCooldown(COOLDOWN_SECS); return 0; }
      return next;
    });

    setTimeout(() => setIsWorking(false), 1000);
  };

  return (
    <div className="village-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <h1 className="text-gradient" style={{ fontSize: '2rem' }}>마을</h1>
      <p style={{ color: 'var(--text-muted)' }}>안전한 마을입니다. 이곳에서 정비하고 성장하세요.</p>

      <div className="card-grid">
        {/* 훈련소 */}
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }} style={{ position: 'relative' }}>
          <div className="stat-header">
            <h3><Dumbbell size={20} style={{ display: 'inline', verticalAlign: 'middle', marginRight: '8px' }} />훈련소</h3>
            {trainCooldown > 0 && <span style={{ fontSize: '0.8rem', color: 'var(--danger)' }}>대기: {trainCooldown}초</span>}
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>50G를 소모하여 무작위 스탯을 1 올립니다.</p>
          <button
            className={`btn ${gold >= 50 ? 'btn-primary' : 'btn-danger'}`}
            style={{ marginTop: '10px', opacity: trainCooldown > 0 ? 0.5 : 1, cursor: trainCooldown > 0 ? 'not-allowed' : 'pointer' }}
            onClick={handleTrain}
            disabled={trainCooldown > 0}
          >
            {trainCooldown > 0 ? '훈련 휴식 중' : '무작위 훈련 (50G)'}
          </button>
          {/* 훈련 성공 시 스탯 이름 팝업 */}
          <AnimatePresence>
            {trainResult && (
              <motion.div
                initial={{ opacity: 0, y: 10, scale: 0.5 }}
                animate={{ opacity: 1, y: -20, scale: 1 }}
                exit={{ opacity: 0, y: -40, scale: 1.5 }}
                style={{ position: 'absolute', top: '50%', left: '50%', transform: 'translate(-50%, -50%)', background: 'var(--primary)', color: '#000', padding: '10px 20px', borderRadius: '20px', fontWeight: 'bold', display: 'flex', alignItems: 'center', gap: '5px', pointerEvents: 'none', zIndex: 10 }}
              >
                <ArrowUpCircle size={20} />
                {trainResult} +1
              </motion.div>
            )}
          </AnimatePresence>
        </motion.div>

        {/* 여관 */}
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }}>
          <div className="stat-header">
            <h3><HeartPulse size={20} style={{ display: 'inline', verticalAlign: 'middle', marginRight: '8px', color: 'var(--danger)' }} />여관</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>50G를 지불하고 체력과 마나를 모두 회복합니다.</p>
          <button
            className={`btn ${gold >= 50 ? 'btn-primary' : 'btn-danger'}`}
            style={{ marginTop: '10px' }}
            onClick={handleHeal}
          >
            휴식하기 (50G)
          </button>
        </motion.div>

        {/* 마을 알바 */}
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }}>
          <div className="stat-header">
            <h3><Coins size={20} style={{ display: 'inline', verticalAlign: 'middle', marginRight: '8px', color: '#fbbf24' }} />마을 알바</h3>
            {jobCooldown > 0
              ? <span style={{ fontSize: '0.8rem', color: 'var(--danger)' }}>대기: {jobCooldown}초</span>
              : <span style={{ fontSize: '0.8rem', color: 'var(--text-muted)' }}>남은 횟수: {MAX_JOB_STREAK - jobCount}/{MAX_JOB_STREAK}</span>
            }
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>잡일을 도와주고 약간의 골드를 획득합니다.</p>
          <button
            className="btn"
            disabled={isWorking || jobCooldown > 0}
            style={{ marginTop: '10px', background: 'rgba(251, 191, 36, 0.1)', color: '#fbbf24', border: '1px solid rgba(251, 191, 36, 0.3)', opacity: (isWorking || jobCooldown > 0) ? 0.5 : 1, cursor: (isWorking || jobCooldown > 0) ? 'not-allowed' : 'pointer' }}
            onClick={workJob}
          >
            {jobCooldown > 0 ? '알바 휴식 중' : isWorking ? '알바 중...' : '알바하기'}
          </button>
        </motion.div>

        {/* 상점 */}
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }}>
          <div className="stat-header">
            <h3><ShoppingCart size={20} style={{ display: 'inline', verticalAlign: 'middle', marginRight: '8px', color: '#34d399' }} />상점</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>새로운 장비와 물약을 구매합니다.</p>
          <button
            className="btn btn-primary"
            style={{ marginTop: '10px', background: 'rgba(52, 211, 153, 0.1)', color: '#34d399', border: '1px solid rgba(52, 211, 153, 0.3)' }}
            onClick={() => setActiveTab('shop')}
          >
            입장하기
          </button>
        </motion.div>

        {/* 대장간 */}
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }}>
          <div className="stat-header">
            <h3><Hammer size={20} style={{ display: 'inline', verticalAlign: 'middle', marginRight: '8px' }} />대장간</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>장비 수리 및 아이템 제작을 수행합니다.</p>
          <button className="btn" style={{ marginTop: '10px' }} onClick={() => setActiveTab('blacksmith')}>
            입장하기
          </button>
        </motion.div>

        {/* 정보 상인 */}
        <motion.div className="stat-card glass-panel text-center">
          <div className="stat-header" style={{ justifyContent: 'center', marginBottom: '15px' }}>
            <h3 style={{ display: 'flex', alignItems: 'center', gap: '8px' }}>
              <BookOpen size={20} className="text-secondary" /> 정보 상인
            </h3>
          </div>
          <p style={{ fontSize: '0.9rem', color: 'var(--text-muted)', marginBottom: '15px' }}>
            500G를 내면 전설적인 영웅들의 전직 비밀을 알려줍니다.
          </p>
          <button
            className={`btn ${gold >= 500 ? '' : 'btn-danger'}`}
            style={{ width: '100%', borderColor: 'var(--secondary)', color: 'var(--secondary)' }}
            onClick={() => dispatch({ type: 'BUY_HINT' })}
          >
            힌트 듣기 (500G)
          </button>
        </motion.div>
      </div>
    </div>
  );
}
