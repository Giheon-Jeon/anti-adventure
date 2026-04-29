import { motion } from 'framer-motion';
import { Hammer, ArrowLeft } from 'lucide-react';

export default function BlacksmithView({ state, dispatch, onBack }) {
  const repairAll = () => {
    dispatch({ type: 'REPAIR_ALL', payload: { cost: 100 } });
  };

  const attemptCraft = () => {
    dispatch({ type: 'ADD_LOG', payload: '장비 제작 기능은 현재 개발 중입니다! (Coming Soon)' });
  }

  return (
    <div className="shop-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: '10px' }}>
        <button className="btn" onClick={onBack} style={{ padding: '8px' }}><ArrowLeft size={20} /></button>
        <h1 className="text-gradient" style={{ fontSize: '2rem', margin: 0 }}>대장간</h1>
      </div>
      <p style={{ color: 'var(--text-muted)' }}>장비를 수리하거나 강력한 아이템을 제작하세요. (현재 소지 골드: <strong style={{color: '#fbbf24'}}>{state.player.gold}G</strong>)</p>

      <div className="card-grid">
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }}>
          <div className="stat-header">
            <h3><Hammer size={16} style={{marginRight: '5px'}}/>모든 장비 수리</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginTop: '10px' }}>손상된 장비의 내구도를 완전히 복구합니다.</p>
          <button className="btn btn-primary" style={{ marginTop: '15px' }} onClick={repairAll}>
            100G 수리
          </button>
        </motion.div>
        
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }}>
          <div className="stat-header">
            <h3><Hammer size={16} style={{marginRight: '5px'}}/>장비 제작 (업데이트 예정)</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginTop: '10px' }}>몬스터 전리품을 이용해 강력한 장비를 만듭니다.</p>
          <button className="btn" style={{ marginTop: '15px', background: 'transparent', border: '1px solid var(--border)' }} onClick={attemptCraft}>
            제작 시도
          </button>
        </motion.div>
      </div>
    </div>
  );
}
