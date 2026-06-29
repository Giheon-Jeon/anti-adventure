/**
 * CharacterView - 캐릭터 정보 화면
 *
 * 표시 항목:
 * - 전투 능력치 (STR/DEX/INT/LUK + 유니온 보너스)
 * - 전투력(CP) 합산
 * - 최근 행동 로그
 * - 보유 스킬 목록 및 스킬 업그레이드
 * - 인벤토리(재료 아이템)
 *
 * 최소 권한 props:
 * - player:     현재 캐릭터 (스탯·스킬·인벤토리)
 * - unionBonus: 비활성 슬롯 유니온 보너스
 * - logs:       최근 행동 로그
 * - dispatch
 */
import { Activity, Shield, Zap, Target } from 'lucide-react';
import { motion } from 'framer-motion';

/** 스탯별 아이콘 매핑 */
const STAT_ICONS = {
  str: <Activity className="text-danger"    size={18} />,
  dex: <Target   className="text-accent"    size={18} />,
  int: <Zap      className="text-primary"   size={18} />,
  luk: <Shield   className="text-secondary" size={18} />,
};

/**
 * 전투력(CP) 계산식.
 * STR: ×2, DEX·INT: ×1.5, LUK: ×1 (유니온 포함)
 */
const calcCP = (stats, ub) =>
  (stats.str + (ub?.str || 0)) * 2
  + (stats.dex + (ub?.dex || 0)) * 1.5
  + (stats.int + (ub?.int || 0)) * 1.5
  + (stats.luk + (ub?.luk || 0));

/**
 * @param {{
 *   player:     Object,
 *   unionBonus: Object,
 *   logs:       string[],
 *   dispatch:   Function
 * }} props
 */
export default function CharacterView({ player, unionBonus, logs, dispatch }) {
  const { stats } = player;
  const ub        = unionBonus || {};

  return (
    <div className="character-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <h1 className="text-gradient" style={{ fontSize: '2rem' }}>캐릭터 정보</h1>

      <div className="card-grid">
        {/* ── 전투 능력치 ──────────────────────────────────────────────── */}
        <motion.div
          className="stat-card glass-panel"
          initial={{ scale: 0.9, opacity: 0 }}
          animate={{ scale: 1, opacity: 1 }}
          transition={{ delay: 0.1 }}
        >
          <div className="stat-header">
            <h3>전투 능력치 <span style={{ fontSize: '0.8rem', color: 'var(--primary)', fontWeight: 'normal' }}>(+유니온)</span></h3>
          </div>

          {['str', 'dex', 'int', 'luk'].map(key => (
            <div key={key} className="stat-row">
              <span>{{str:'힘 (STR)', dex:'민첩 (DEX)', int:'지능 (INT)', luk:'행운 (LUK)'}[key]}</span>
              <span style={{ display: 'flex', alignItems: 'center', gap: '5px' }}>
                {STAT_ICONS[key]} {stats[key]}
                {ub[key] > 0 && <span style={{ color: 'var(--primary)', fontSize: '0.8rem' }}>+{ub[key]}</span>}
              </span>
            </div>
          ))}

          {/* 전투력 합산 */}
          <div style={{ marginTop: '15px', paddingTop: '15px', borderTop: '1px solid var(--glass-border)' }}>
            <div className="stat-row">
              <span style={{ color: 'var(--text-muted)' }}>전투력 (CP)</span>
              <strong style={{ fontSize: '1.2rem', color: 'var(--accent)' }}>
                {calcCP(stats, ub)}
              </strong>
            </div>
          </div>
        </motion.div>

        {/* ── 최근 기록 ────────────────────────────────────────────────── */}
        <motion.div
          className="stat-card glass-panel"
          initial={{ scale: 0.9, opacity: 0 }}
          animate={{ scale: 1, opacity: 1 }}
          transition={{ delay: 0.2 }}
        >
          <div className="stat-header">
            <h3>최근 기록</h3>
          </div>
          <div
            style={{ display: 'flex', flexDirection: 'column', gap: '8px', maxHeight: '200px', overflowY: 'auto', paddingRight: '5px' }}
            className="scrollbar-hide"
          >
            {logs.length === 0 ? (
              <p style={{ color: 'var(--text-muted)' }}>기록이 없습니다.</p>
            ) : (
              logs.map((log, i) => (
                <div
                  key={i}
                  style={{
                    fontSize: '0.85rem',
                    padding: '8px',
                    background: 'rgba(0,0,0,0.2)',
                    borderRadius: '6px',
                    color: i === 0 ? 'var(--text-main)' : 'var(--text-muted)', // 가장 최근 로그만 강조
                  }}
                >
                  {log}
                </div>
              ))
            )}
          </div>
        </motion.div>

        {/* ── 보유 스킬 ────────────────────────────────────────────────── */}
        <motion.div
          className="stat-card glass-panel"
          style={{ gridColumn: '1 / -1' }}
          initial={{ scale: 0.9, opacity: 0 }}
          animate={{ scale: 1, opacity: 1 }}
          transition={{ delay: 0.3 }}
        >
          <div className="stat-header">
            <h3>보유 스킬</h3>
            <span style={{ fontSize: '0.8rem', color: 'var(--text-muted)' }}>
              잔여 포인트: <strong style={{ color: 'var(--primary)' }}>{player.skill_points}</strong>
            </span>
          </div>
          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(250px, 1fr))', gap: '15px' }}>
            {player.skills?.map(skill => {
              const canUpgrade = player.skill_points > 0 && skill.level < skill.maxLevel;
              return (
                <div
                  key={skill.id}
                  style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', background: 'rgba(0,0,0,0.2)', padding: '15px', borderRadius: '8px' }}
                >
                  <div style={{ flex: 1, marginRight: '10px' }}>
                    <div style={{ fontWeight: 'bold', fontSize: '1rem' }}>
                      {skill.name} <span style={{ color: 'var(--primary)', fontSize: '0.9rem' }}>Lv.{skill.level}/{skill.maxLevel}</span>
                    </div>
                    <div style={{ fontSize: '0.75rem', color: 'var(--text-muted)', marginTop: '4px' }}>{skill.desc}</div>
                  </div>
                  <button
                    className="btn"
                    style={{ padding: '8px 15px', background: 'var(--primary)', color: '#000', fontWeight: 'bold', opacity: canUpgrade ? 1 : 0.3, cursor: canUpgrade ? 'pointer' : 'not-allowed' }}
                    onClick={() => dispatch({ type: 'UPGRADE_SKILL', payload: skill.id })}
                    disabled={!canUpgrade}
                  >
                    +
                  </button>
                </div>
              );
            })}
          </div>
        </motion.div>

        {/* ── 인벤토리 ─────────────────────────────────────────────────── */}
        <motion.div
          className="stat-card glass-panel"
          style={{ gridColumn: '1 / -1' }}
          initial={{ scale: 0.9, opacity: 0 }}
          animate={{ scale: 1, opacity: 1 }}
          transition={{ delay: 0.4 }}
        >
          <div className="stat-header">
            <h3>🎒 가방 (인벤토리)</h3>
            <span style={{ fontSize: '0.8rem', color: 'var(--text-muted)' }}>보유한 전리품 재료</span>
          </div>
          {(!player.inventory || player.inventory.length === 0) ? (
            <p style={{ color: 'var(--text-muted)', fontSize: '0.9rem', textAlign: 'center', padding: '20px 0' }}>
              비어 있음 (던전에서 몬스터를 처치하여 재료를 획득하세요!)
            </p>
          ) : (
            <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fill, minmax(160px, 1fr))', gap: '12px' }}>
              {player.inventory.map(item => (
                <div
                  key={item.name}
                  style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', background: 'rgba(0,0,0,0.2)', padding: '10px 15px', borderRadius: '8px', border: '1px solid var(--glass-border)' }}
                >
                  <span style={{ fontSize: '0.9rem', fontWeight: '500' }}>{item.name}</span>
                  <strong style={{ color: 'var(--accent)', fontSize: '0.95rem' }}>{item.count}개</strong>
                </div>
              ))}
            </div>
          )}
        </motion.div>
      </div>
    </div>
  );
}
