import { Coins } from 'lucide-react';

export default function TopBar({ player }) {
  const expPercent = (player.exp / player.maxExp) * 100;
  const hpPercent = (player.hp / player.maxHp) * 100;
  const mpPercent = (player.mp / player.maxMp) * 100;

  return (
    <header className="top-bar glass-panel">
      <div className="player-info">
        <div className="player-avatar">
          {player.name.charAt(0)}
        </div>
        <div className="player-stats">
          <h2>{player.name} <span className="text-gradient">Lv.{player.level}</span></h2>
          <p>{player.job}</p>
        </div>
      </div>

      <div className="bars-container" style={{ flex: 1, margin: '0 30px' }}>
        <div style={{ display: 'flex', gap: '15px', marginBottom: '8px' }}>
          <div style={{ flex: 1 }}>
            <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.8rem' }}>
              <span>HP</span>
              <span>{player.hp}/{player.maxHp}</span>
            </div>
            <div className="progress-bar-bg">
              <div className="progress-bar-fill hp-fill" style={{ width: `${hpPercent}%` }} />
            </div>
          </div>
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

      <div className="resource-bar">
        <div className="resource-item gold-text">
          <Coins size={20} />
          {player.gold.toLocaleString()} G
        </div>
      </div>
    </header>
  );
}
