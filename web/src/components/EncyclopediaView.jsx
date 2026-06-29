/**
 * EncyclopediaView - 몬스터 도감 화면
 *
 * 처치한 몬스터마다 연구 레벨이 오르며 보너스를 제공합니다.
 * - 연구 Lv.1: 기본 정보 (첫 처치 시 자동)
 * - 연구 Lv.2: 데미지 +20% (10회 처치 시 해금)
 * - 연구 Lv.3: 자동 사냥 / 약점 노출 (30회 처치 시 해금)
 *
 * 최소 권한 props:
 * - encyclopedia: 처치 기록 배열 [{ name, kills }]
 */
import { BookOpen } from 'lucide-react';
import { motion } from 'framer-motion';

/** 처치 수에 따른 연구 레벨 */
const getResearchLevel = (kills) => {
  if (kills >= 30) return 3;
  if (kills >= 10) return 2;
  return 1;
};

/**
 * @param {{ encyclopedia: Array<{ name: string, kills: number }> }} props
 */
export default function EncyclopediaView({ encyclopedia }) {
  return (
    <div className="encyclopedia-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <h1 className="text-gradient" style={{ fontSize: '2rem' }}>몬스터 도감</h1>
      <p style={{ color: 'var(--text-muted)' }}>처치한 몬스터들의 기록입니다. 처치 횟수가 오를수록 전투에 유리해집니다.</p>

      {encyclopedia.length === 0 ? (
        <div className="glass-panel" style={{ padding: '40px', textAlign: 'center', color: 'var(--text-muted)' }}>
          <BookOpen size={48} style={{ margin: '0 auto', marginBottom: '15px', opacity: 0.5 }} />
          <p>아직 발견한 몬스터가 없습니다.</p>
        </div>
      ) : (
        <div className="card-grid">
          {encyclopedia.map((entry, idx) => {
            const researchLevel = getResearchLevel(entry.kills);

            return (
              <motion.div
                key={entry.name}
                className="stat-card glass-panel"
                initial={{ opacity: 0, y: 20 }}
                animate={{ opacity: 1, y: 0 }}
                transition={{ delay: idx * 0.1 }}
              >
                <div className="stat-header">
                  <h3 style={{ fontSize: '1.2rem', color: 'var(--primary)' }}>{entry.name}</h3>
                  <span style={{ background: 'rgba(255,255,255,0.1)', padding: '2px 8px', borderRadius: '12px', fontSize: '0.8rem' }}>
                    연구 Lv.{researchLevel}
                  </span>
                </div>

                <div style={{ marginTop: '10px' }}>
                  <div className="stat-row" style={{ marginBottom: '5px' }}>
                    <span style={{ color: 'var(--text-muted)' }}>누적 처치 수</span>
                    <strong>{entry.kills}회</strong>
                  </div>

                  {/* 연구 레벨별 해금 혜택 표시 */}
                  <div style={{ marginTop: '15px', fontSize: '0.85rem' }}>
                    <p style={{ color: 'var(--text-main)' }}>
                      ✓ 기본 정보 개방
                    </p>
                    <p style={{ color: researchLevel >= 2 ? 'var(--accent)' : 'var(--text-muted)', marginTop: '5px' }}>
                      {researchLevel >= 2 ? '✓ 입히는 데미지 +20%' : '🔒 데미지 +20% (10회 처치 필요)'}
                    </p>
                    <p style={{ color: researchLevel >= 3 ? '#fbbf24' : 'var(--text-muted)', marginTop: '5px' }}>
                      {researchLevel >= 3 ? '✓ 자동 사냥 해금 / 약점 노출' : '🔒 보너스 혜택 (30회 처치 필요)'}
                    </p>
                  </div>
                </div>
              </motion.div>
            );
          })}
        </div>
      )}
    </div>
  );
}
