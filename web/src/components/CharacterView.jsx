import { Activity, Shield, Zap, Target } from 'lucide-react';
import { motion } from 'framer-motion';

export default function CharacterView({ state }) {
  const { player } = state;
  const { stats } = player;

  const statIcons = {
    str: <Activity className="text-danger" size={18} />,
    dex: <Target className="text-accent" size={18} />,
    int: <Zap className="text-primary" size={18} />,
    luk: <Shield className="text-secondary" size={18} />
  };

  return (
    <div className="character-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <h1 className="text-gradient" style={{ fontSize: '2rem' }}>캐릭터 정보</h1>
      
      <div className="card-grid">
        <motion.div 
          className="stat-card glass-panel"
          initial={{ scale: 0.9, opacity: 0 }}
          animate={{ scale: 1, opacity: 1 }}
          transition={{ delay: 0.1 }}
        >
          <div className="stat-header">
            <h3>전투 능력치</h3>
          </div>
          <div className="stat-row"><span>힘 (STR)</span> <span style={{ display: 'flex', alignItems: 'center', gap: '5px' }}>{statIcons.str} {stats.str}</span></div>
          <div className="stat-row"><span>민첩 (DEX)</span> <span style={{ display: 'flex', alignItems: 'center', gap: '5px' }}>{statIcons.dex} {stats.dex}</span></div>
          <div className="stat-row"><span>지능 (INT)</span> <span style={{ display: 'flex', alignItems: 'center', gap: '5px' }}>{statIcons.int} {stats.int}</span></div>
          <div className="stat-row"><span>행운 (LUK)</span> <span style={{ display: 'flex', alignItems: 'center', gap: '5px' }}>{statIcons.luk} {stats.luk}</span></div>
          
          <div style={{ marginTop: '15px', paddingTop: '15px', borderTop: '1px solid var(--glass-border)' }}>
            <div className="stat-row">
              <span style={{ color: 'var(--text-muted)' }}>전투력 (CP)</span>
              <strong style={{ fontSize: '1.2rem', color: 'var(--accent)' }}>
                {stats.str * 2 + stats.dex * 1.5 + stats.int * 1.5 + stats.luk}
              </strong>
            </div>
          </div>
        </motion.div>

        <motion.div 
          className="stat-card glass-panel"
          initial={{ scale: 0.9, opacity: 0 }}
          animate={{ scale: 1, opacity: 1 }}
          transition={{ delay: 0.2 }}
        >
          <div className="stat-header">
            <h3>최근 기록</h3>
          </div>
          <div style={{ display: 'flex', flexDirection: 'column', gap: '8px', maxHeight: '200px', overflowY: 'auto', paddingRight: '5px' }} className="scrollbar-hide">
            {state.logs.length === 0 ? (
              <p style={{ color: 'var(--text-muted)' }}>기록이 없습니다.</p>
            ) : (
              state.logs.map((log, i) => (
                <div key={i} style={{ 
                  fontSize: '0.85rem', 
                  padding: '8px', 
                  background: 'rgba(0,0,0,0.2)', 
                  borderRadius: '6px',
                  color: i === 0 ? 'var(--text-main)' : 'var(--text-muted)'
                }}>
                  {log}
                </div>
              ))
            )}
          </div>
        </motion.div>

        <motion.div 
          className="stat-card glass-panel"
          style={{ gridColumn: '1 / -1' }}
          initial={{ scale: 0.9, opacity: 0 }}
          animate={{ scale: 1, opacity: 1 }}
          transition={{ delay: 0.3 }}
        >
          <div className="stat-header">
            <h3>보유 스킬</h3>
            <span style={{ fontSize: '0.8rem', color: 'var(--text-muted)' }}>잔여 포인트: <strong style={{color: 'var(--primary)'}}>{player.skill_points}</strong></span>
          </div>
          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(250px, 1fr))', gap: '15px' }}>
            {player.skills && player.skills.map(skill => (
              <div key={skill.id} style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', background: 'rgba(0,0,0,0.2)', padding: '15px', borderRadius: '8px' }}>
                <div style={{ flex: 1, marginRight: '10px' }}>
                  <div style={{ fontWeight: 'bold', fontSize: '1rem' }}>{skill.name} <span style={{ color: 'var(--primary)', fontSize: '0.9rem' }}>Lv.{skill.level}/{skill.maxLevel}</span></div>
                  <div style={{ fontSize: '0.75rem', color: 'var(--text-muted)', marginTop: '4px' }}>{skill.desc}</div>
                </div>
                <button 
                  className="btn" 
                  style={{ 
                    padding: '8px 15px', 
                    background: 'var(--primary)', 
                    color: '#000', 
                    fontWeight: 'bold',
                    opacity: (player.skill_points > 0 && skill.level < skill.maxLevel) ? 1 : 0.3, 
                    cursor: (player.skill_points > 0 && skill.level < skill.maxLevel) ? 'pointer' : 'not-allowed' 
                  }}
                  onClick={() => dispatch({ type: 'UPGRADE_SKILL', payload: skill.id })}
                  disabled={player.skill_points <= 0 || skill.level >= skill.maxLevel}
                >
                  +
                </button>
              </div>
            ))}
          </div>
        </motion.div>
      </div>
    </div>
  );
}
