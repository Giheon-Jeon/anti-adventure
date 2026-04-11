import React, { useState } from 'react';
import monsters from './data/monsters.json';
import dungeons from './data/dungeons.json';
import './index.css';

function App() {
  const [activeTab, setActiveTab] = useState('monsters');

  return (
    <div className="dashboard-container">
      <header className="animate-fade-in">
        <h1>ANTI-ADVENTURE</h1>
        <p className="subtitle">TRPG Core Data Engine & Analytics Dashboard</p>
      </header>

      <div className="section-title animate-fade-in">
        <div style={{ display: 'flex', gap: '20px' }}>
          <button 
            onClick={() => setActiveTab('monsters')}
            style={{ 
              background: 'none', 
              border: 'none', 
              color: activeTab === 'monsters' ? '#00d2ff' : '#a0a0c0',
              fontSize: '1.2rem',
              fontWeight: '600',
              cursor: 'pointer',
              borderBottom: activeTab === 'monsters' ? '2px solid #00d2ff' : 'none',
              paddingBottom: '5px'
            }}
          >
            Monster Database
          </button>
          <button 
            onClick={() => setActiveTab('dungeons')}
            style={{ 
              background: 'none', 
              border: 'none', 
              color: activeTab === 'dungeons' ? '#00d2ff' : '#a0a0c0',
              fontSize: '1.2rem',
              fontWeight: '600',
              cursor: 'pointer',
              borderBottom: activeTab === 'dungeons' ? '2px solid #00d2ff' : 'none',
              paddingBottom: '5px'
            }}
          >
            Dungeon Atlas
          </button>
        </div>
      </div>

      <div className="stats-grid">
        {activeTab === 'monsters' ? (
          monsters.map((monster, index) => (
            <div key={monster.id} className="card animate-fade-in" style={{ animationDelay: `${index * 0.05}s` }}>
              <span className="tag">{monster.is_boss ? '🔥 BOSS' : '👾 MOB'}</span>
              <h2 className="monster-name">{monster.name}</h2>
              
              <div className="stat-row">
                <span className="stat-label">Health Points</span>
                <span className="stat-value">{monster.hp}</span>
              </div>
              <div className="progress-bar">
                <div className="progress-fill" style={{ width: `${Math.min(100, (monster.hp / 2000) * 100)}%` }}></div>
              </div>

              <div className="stat-row" style={{ marginTop: '15px' }}>
                <span className="stat-label">Defense</span>
                <span className="stat-value">{monster.def}</span>
              </div>
              <div className="progress-bar">
                <div className="progress-fill" style={{ width: `${Math.min(100, (monster.def / 100) * 100)}%`, background: '#9d50bb' }}></div>
              </div>

              <div style={{ marginTop: '20px', padding: '10px', background: 'rgba(255,255,255,0.05)', borderRadius: '10px' }}>
                <div className="stat-row" style={{ marginBottom: 0 }}>
                  <span className="stat-label">Loot: {monster.loot.name}</span>
                  <span className="stat-value" style={{ color: '#ffd700' }}>{monster.loot.value} G</span>
                </div>
              </div>
            </div>
          ))
        ) : (
          dungeons.map((dungeon, index) => (
            <div key={index} className="card animate-fade-in" style={{ animationDelay: `${index * 0.1}s` }}>
              <span className="tag" style={{ background: 'rgba(157, 80, 187, 0.1)', color: '#9d50bb', borderColor: 'rgba(157, 80, 187, 0.2)' }}>
                {dungeon.has_boss ? '🏰 BOSS DUNGEON' : '🌲 EXPLORATION'}
              </span>
              <h2 className="monster-name">{dungeon.name}</h2>
              
              <div className="stat-row">
                <span className="stat-label">Required CP</span>
                <span className="stat-value">{dungeon.min_cp} ~ {dungeon.max_cp}</span>
              </div>
              
              <div style={{ marginTop: '15px' }}>
                <span className="stat-label" style={{ fontSize: '0.8rem' }}>Resident Monsters:</span>
                <div style={{ display: 'flex', gap: '5px', flexWrap: 'wrap', marginTop: '5px' }}>
                  {dungeon.monster_ids.map(mid => (
                    <span key={mid} style={{ fontSize: '0.7rem', padding: '2px 8px', background: 'rgba(255,255,255,0.1)', borderRadius: '4px' }}>
                      ID: {mid}
                    </span>
                  ))}
                  {dungeon.has_boss && (
                    <span style={{ fontSize: '0.7rem', padding: '2px 8px', background: 'rgba(255, 80, 80, 0.2)', borderRadius: '4px', color: '#ff5050' }}>
                      Boss ID: {dungeon.boss_id}
                    </span>
                  )}
                </div>
              </div>
            </div>
          ))
        )}
      </div>
    </div>
  );
}

export default App;
