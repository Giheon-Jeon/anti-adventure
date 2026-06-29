/**
 * TopBar - 상단 고정 플레이어 정보 바
 *
 * 항상 화면 상단에 표시되며 현재 캐릭터의 핵심 자원을 나타냅니다.
 * - 아바타(이름 첫 글자), 이름, 레벨, 직업
 * - HP / MP / EXP 게이지
 * - 골드
 *
 * 최소 권한 props:
 * - player:     name, level, job, hp, maxHp, mp, maxMp, exp, maxExp, gold
 * - unionBonus: maxHp (실제 최대 HP 계산용)
 */
import { Coins } from 'lucide-react';

/**
 * @param {{
 *   player:     Object,
 *   unionBonus: Object
 * }} props
 */
export default function TopBar({ player, unionBonus }) {
  // 유니온 보너스로 증가된 실제 최대 HP
  const realMaxHp = player.maxHp + (unionBonus?.maxHp || 0);

  const hpPercent  = (player.hp  / realMaxHp)    * 100;
  const mpPercent  = (player.mp  / player.maxMp)  * 100;
  const expPercent = (player.exp / player.maxExp) * 100;

  return (
    <header className="top-bar glass-panel">
      {/* 아바타 + 이름/직업 */}
      <div className="player-info">
        <div className="player-avatar">
          {player.name.charAt(0)}
        </div>
        <div className="player-stats">
          <h2>{player.name} <span className="text-gradient">Lv.{player.level}</span></h2>
          <p>{player.job}</p>
        </div>
      </div>

      {/* HP / MP / EXP 게이지 */}
      <div className="bars-container" style={{ flex: 1, margin: '0 30px' }}>
        <div style={{ display: 'flex', gap: '15px', marginBottom: '8px' }}>
          {/* HP 게이지 */}
          <div style={{ flex: 1 }}>
            <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.8rem' }}>
              <span>HP</span>
              <span>{player.hp}/{realMaxHp}</span>
            </div>
            <div className="progress-bar-bg">
              <div className="progress-bar-fill hp-fill" style={{ width: `${hpPercent}%` }} />
            </div>
          </div>

          {/* MP 게이지 */}
          <div style={{ flex: 1 }}>
            <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.8rem' }}>
              <span>MP</span>
              <span>{player.mp}/{player.maxMp}</span>
            </div>
            <div className="progress-bar-bg">
              <div className="progress-bar-fill mp-fill" style={{ width: `${mpPercent}%`, background: '#3b82f6' }} />
            </div>
          </div>
        </div>

        {/* EXP 게이지 */}
        <div>
          <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.8rem' }}>
            <span>EXP</span>
            <span>{player.exp} / {player.maxExp}</span>
          </div>
          <div className="progress-bar-bg">
            <div className="progress-bar-fill exp-fill" style={{ width: `${expPercent}%` }} />
          </div>
        </div>
      </div>

      {/* 골드 */}
      <div className="resource-bar">
        <div className="resource-item gold-text">
          <Coins size={20} />
          {player.gold.toLocaleString()} G
        </div>
      </div>
    </header>
  );
}
