import { useState, useEffect } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { Shield, Sword, Map, Home, Book, Settings, User } from 'lucide-react';
import './App.css';

// We will implement individual views in separate components
import VillageView from './components/VillageView';
import ShopView from './components/ShopView';
import BlacksmithView from './components/BlacksmithView';
import DungeonView from './components/DungeonView';
import CharacterView from './components/CharacterView';
import EncyclopediaView from './components/EncyclopediaView';
import TopBar from './components/TopBar';

import { useGameState } from './hooks/useGameState';

function App() {
  const [activeTab, setActiveTab] = useState('character');
  const { gameState, dispatch } = useGameState();

  const tabs = [
    { id: 'character', icon: <User size={20} />, label: '캐릭터' },
    { id: 'village', icon: <Home size={20} />, label: '마을' },
    { id: 'dungeon', icon: <Map size={20} />, label: '던전' },
    { id: 'encyclopedia', icon: <Book size={20} />, label: '도감' },
  ];

  return (
    <div className="app-container">
      <TopBar player={gameState.player} />
      
      <main className="main-content">
        <AnimatePresence mode="wait">
          <motion.div
            key={activeTab}
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            exit={{ opacity: 0, y: -20 }}
            transition={{ duration: 0.3 }}
            className="view-container"
          >
            {activeTab === 'character' && <CharacterView state={gameState} dispatch={dispatch} />}
            {activeTab === 'village' && <VillageView state={gameState} dispatch={dispatch} setActiveTab={setActiveTab} />}
            {activeTab === 'shop' && <ShopView state={gameState} dispatch={dispatch} onBack={() => setActiveTab('village')} />}
            {activeTab === 'blacksmith' && <BlacksmithView state={gameState} dispatch={dispatch} onBack={() => setActiveTab('village')} />}
            {activeTab === 'dungeon' && <DungeonView state={gameState} dispatch={dispatch} />}
            {activeTab === 'encyclopedia' && <EncyclopediaView state={gameState} dispatch={dispatch} />}
          </motion.div>
        </AnimatePresence>
      </main>

      <nav className="bottom-nav glass-panel">
        {tabs.map(tab => (
          <button
            key={tab.id}
            className={`nav-btn ${activeTab === tab.id ? 'active' : ''}`}
            onClick={() => setActiveTab(tab.id)}
          >
            {tab.icon}
            <span>{tab.label}</span>
            {activeTab === tab.id && (
              <motion.div className="active-indicator" layoutId="active-indicator" />
            )}
          </button>
        ))}
      </nav>
    </div>
  );
}

export default App;
