import { useState } from 'react';
import { motion } from 'framer-motion';
import { User, Trash2, Edit3, ShieldAlert } from 'lucide-react';

export default function SettingsView({ state, dispatch }) {
  const { characters, activeSlot, player, unionBonus } = state;
  const [editingName, setEditingName] = useState(false);
  const [newName, setNewName] = useState(player.name);

  const handleNameChange = (e) => {
    e.preventDefault();
    if (newName.trim()) {
      dispatch({ type: 'CHANGE_NAME', payload: newName.trim() });
      setEditingName(false);
    }
  };

  const handleSwitch = (slotIndex) => {
    if (slotIndex !== activeSlot) {
      dispatch({ type: 'SWITCH_SLOT', payload: slotIndex });
    }
  };

  const handleReset = (slotIndex) => {
    if (window.confirm(`정말 슬롯 ${slotIndex + 1}의 캐릭터를 초기화하시겠습니까? (이 작업은 되돌릴 수 없습니다)`)) {
      dispatch({ type: 'RESET_SLOT', payload: slotIndex });
    }
  };

  return (
    <div className="settings-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <h1 className="text-gradient" style={{ fontSize: '2rem' }}>설정 (유니온)</h1>
      <p style={{ color: 'var(--text-muted)' }}>가문의 모든 캐릭터의 레벨 총합에 따라 부가 능력치 혜택을 얻습니다.</p>

      <div className="card-grid">
        {/* Union Status */}
        <motion.div className="stat-card glass-panel" style={{ gridColumn: '1 / -1', border: '1px solid var(--primary)' }}>
          <div className="stat-header">
            <h3>🛡️ 유니온 효과</h3>
            <span style={{ fontSize: '0.9rem', color: 'var(--text-muted)' }}>총합 레벨: <strong style={{ color: 'var(--primary)' }}>{unionBonus.totalLevel}</strong></span>
          </div>
          <div style={{ marginTop: '10px', fontSize: '0.9rem', color: 'var(--text-main)', display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '10px' }}>
            <div style={{ background: 'rgba(0,0,0,0.2)', padding: '10px', borderRadius: '8px' }}>
              <strong>전투 능력치 보너스</strong>
              <div style={{ color: 'var(--primary)', marginTop: '5px' }}>모든 스탯 (STR, DEX, INT, LUK) +{unionBonus.str}</div>
            </div>
            <div style={{ background: 'rgba(0,0,0,0.2)', padding: '10px', borderRadius: '8px' }}>
              <strong>체력 보너스</strong>
              <div style={{ color: 'var(--danger)', marginTop: '5px' }}>최대 HP +{unionBonus.maxHp}</div>
            </div>
          </div>
        </motion.div>

        {/* Current Character Name */}
        <motion.div className="stat-card glass-panel" style={{ gridColumn: '1 / -1' }}>
          <div className="stat-header">
            <h3>현재 캐릭터 이름 변경</h3>
          </div>
          {editingName ? (
            <form onSubmit={handleNameChange} style={{ display: 'flex', gap: '10px', marginTop: '10px' }}>
              <input 
                type="text" 
                value={newName} 
                onChange={(e) => setNewName(e.target.value)}
                style={{ flex: 1, padding: '10px', borderRadius: '8px', border: '1px solid var(--border)', background: 'rgba(0,0,0,0.3)', color: 'white' }}
                maxLength={10}
              />
              <button type="submit" className="btn btn-primary" style={{ padding: '0 20px' }}>저장</button>
              <button type="button" className="btn" onClick={() => { setEditingName(false); setNewName(player.name); }}>취소</button>
            </form>
          ) : (
            <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginTop: '10px', background: 'rgba(0,0,0,0.2)', padding: '15px', borderRadius: '8px' }}>
              <strong style={{ fontSize: '1.2rem' }}>{player.name}</strong>
              <button className="btn" style={{ padding: '8px 15px', display: 'flex', gap: '5px', alignItems: 'center' }} onClick={() => setEditingName(true)}>
                <Edit3 size={16} /> 변경하기
              </button>
            </div>
          )}
        </motion.div>

        {/* Slots */}
        {characters && characters.map((char, index) => (
          <motion.div key={index} className="stat-card glass-panel" style={{ opacity: (!char && index !== activeSlot) ? 0.6 : 1, border: activeSlot === index ? '2px solid var(--accent)' : '1px solid var(--glass-border)' }}>
            <div className="stat-header">
              <h3>슬롯 {index + 1} {activeSlot === index && <span style={{ fontSize: '0.8rem', color: 'var(--accent)', marginLeft: '5px' }}>(접속 중)</span>}</h3>
            </div>
            {char ? (
              <div style={{ marginTop: '10px' }}>
                <div style={{ fontSize: '1.2rem', fontWeight: 'bold' }}>{char.name}</div>
                <div style={{ fontSize: '0.9rem', color: 'var(--text-muted)' }}>Lv.{char.level} {char.job}</div>
                <div style={{ marginTop: '15px', display: 'flex', gap: '10px' }}>
                  {activeSlot !== index && (
                    <button className="btn btn-primary" style={{ flex: 1, padding: '8px' }} onClick={() => handleSwitch(index)}>
                      <User size={16} style={{ marginRight: '5px', verticalAlign: 'middle' }}/> 접속
                    </button>
                  )}
                  <button className="btn btn-danger" style={{ flex: activeSlot === index ? 1 : 0, padding: '8px', minWidth: '40px' }} onClick={() => handleReset(index)}>
                    <Trash2 size={16} style={activeSlot === index ? { marginRight: '5px', verticalAlign: 'middle' } : { margin: '0 auto' }}/>
                    {activeSlot === index ? '캐릭터 삭제' : ''}
                  </button>
                </div>
              </div>
            ) : (
              <div style={{ marginTop: '10px', textAlign: 'center', padding: '10px 0' }}>
                <div style={{ color: 'var(--text-muted)', marginBottom: '15px' }}>빈 슬롯</div>
                <button className="btn btn-primary" style={{ width: '100%', padding: '8px' }} onClick={() => handleSwitch(index)}>
                  새 캐릭터 생성
                </button>
              </div>
            )}
          </motion.div>
        ))}
      </div>
    </div>
  );
}
