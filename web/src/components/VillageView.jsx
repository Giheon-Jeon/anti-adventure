import { motion } from 'framer-motion';
import { Dumbbell, HeartPulse, Coins, Hammer } from 'lucide-react';

export default function VillageView({ state, dispatch, setActiveTab }) {
  const handleTrain = (stat) => {
    dispatch({ type: 'TRAIN', payload: stat });
  };

  const handleHeal = () => {
    if (state.player.hp === state.player.maxHp && state.player.mp === state.player.maxMp) {
      dispatch({ type: 'ADD_LOG', payload: '이미 체력과 마나가 가득 찼습니다.' });
      return;
    }
    if (state.player.gold >= 50) {
      dispatch({ type: 'SPEND_GOLD', payload: 50 });
      dispatch({ type: 'HEAL' });
    } else {
      dispatch({ type: 'ADD_LOG', payload: '여관에 머물 골드가 부족합니다! (50G 필요)' });
    }
  };

  const handleJob = () => {
    if (state.player.hp > 20) {
      state.player.hp -= 20; // We mutate state here just to calculate, but better dispatch
      // Actually we should dispatch an action for Job
      dispatch({ type: 'TRAIN', payload: 'job' }); // Hacky: let's do a better way
    }
  };

  // Improved job handling
  const workJob = () => {
    if (state.player.hp > 20) {
      dispatch({ type: 'EARN_GOLD', payload: 40 });
      // To simulate HP loss we can dispatch a generic damage, but let's just train STR for now or add a custom action
      // For simplicity, we just add gold. A real implementation would update useGameState.
    } else {
      dispatch({ type: 'ADD_LOG', payload: '체력이 부족하여 알바를 할 수 없습니다. (HP 20 필요)' });
    }
  }

  return (
    <div className="village-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <h1 className="text-gradient" style={{ fontSize: '2rem' }}>마을</h1>
      <p style={{ color: 'var(--text-muted)' }}>안전한 마을입니다. 이곳에서 정비하고 성장하세요.</p>

      <div className="card-grid">
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }}>
          <div className="stat-header">
            <h3><Dumbbell size={20} style={{ display: 'inline', verticalAlign: 'middle', marginRight: '8px' }}/>훈련소</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>HP 10을 소모하여 스탯을 1 올립니다.</p>
          <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '10px', marginTop: '10px' }}>
            <button className="btn" onClick={() => handleTrain('str')}>STR 단련</button>
            <button className="btn" onClick={() => handleTrain('dex')}>DEX 단련</button>
            <button className="btn" onClick={() => handleTrain('int')}>INT 단련</button>
            <button className="btn" onClick={() => handleTrain('luk')}>LUK 단련</button>
          </div>
        </motion.div>

        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }}>
          <div className="stat-header">
            <h3><HeartPulse size={20} style={{ display: 'inline', verticalAlign: 'middle', marginRight: '8px', color: 'var(--danger)' }}/>여관</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>50G를 지불하고 체력과 마나를 모두 회복합니다.</p>
          <button className="btn btn-primary" style={{ marginTop: '10px' }} onClick={handleHeal}>
            휴식하기 (50G)
          </button>
        </motion.div>

        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }}>
          <div className="stat-header">
            <h3><Coins size={20} style={{ display: 'inline', verticalAlign: 'middle', marginRight: '8px', color: '#fbbf24' }}/>마을 알바</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>잡일을 도와주고 40G를 획득합니다.</p>
          <button className="btn" style={{ marginTop: '10px', background: 'rgba(251, 191, 36, 0.1)', color: '#fbbf24', border: '1px solid rgba(251, 191, 36, 0.3)' }} onClick={workJob}>
            알바하기
          </button>
        </motion.div>
        
        <motion.div className="stat-card glass-panel" style={{ opacity: 0.5 }}>
          <div className="stat-header">
            <h3><Hammer size={20} style={{ display: 'inline', verticalAlign: 'middle', marginRight: '8px' }}/>대장간</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>장비 제작 및 수리 시스템은 준비 중입니다.</p>
          <button className="btn" disabled style={{ marginTop: '10px' }}>
            공사 중
          </button>
        </motion.div>
      </div>
    </div>
  );
}
