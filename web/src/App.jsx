/**
 * App.jsx - 최상위 컴포넌트
 *
 * 역할:
 * 1. useGameState 훅으로 게임 상태 구독
 * 2. 탭 네비게이션 상태 관리
 * 3. 각 뷰 컴포넌트에 필요한 최소 props만 전달 (최소 권한 원칙)
 *
 * 탭 구성: 캐릭터 / 마을 / 던전 / 도감 / 설정
 * 상점·대장간은 마을 탭 내부에서 전환되므로 탭 바에 표시하지 않음.
 */
import { useState } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { User, Home, Map, Book, Settings } from 'lucide-react';
import './App.css';

import TopBar        from './components/TopBar';
import CharacterView from './components/CharacterView';
import VillageView   from './components/VillageView';
import ShopView      from './components/ShopView';
import BlacksmithView from './components/BlacksmithView';
import DungeonView   from './components/DungeonView';
import EncyclopediaView from './components/EncyclopediaView';
import SettingsView  from './components/SettingsView';

import { useGameState } from './hooks/useGameState';

/** 하단 네비게이션 탭 정의 */
const NAV_TABS = [
  { id: 'character',   icon: <User     size={20} />, label: '캐릭터' },
  { id: 'village',     icon: <Home     size={20} />, label: '마을'   },
  { id: 'dungeon',     icon: <Map      size={20} />, label: '던전'   },
  { id: 'encyclopedia',icon: <Book     size={20} />, label: '도감'   },
  { id: 'settings',   icon: <Settings  size={20} />, label: '설정'   },
];

function App() {
  const [activeTab, setActiveTab]   = useState('character');
  const { gameState, dispatch }     = useGameState();

  // 자주 사용하는 값 구조 분해 (props 전달 편의)
  const { player, unionBonus, logs, encyclopedia, dungeons, characters, activeSlot } = gameState;

  return (
    <div className="app-container">
      {/* 상단 바: 플레이어 이름·레벨·HP·MP·EXP·골드 */}
      <TopBar player={player} unionBonus={unionBonus} />

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
            {/* 캐릭터 - 스탯·스킬·인벤토리 */}
            {activeTab === 'character' && (
              <CharacterView
                player={player}
                unionBonus={unionBonus}
                logs={logs}
                dispatch={dispatch}
              />
            )}

            {/* 마을 - 훈련·여관·알바·상점 입장·대장간 입장 */}
            {activeTab === 'village' && (
              <VillageView
                gold={player.gold}
                hp={player.hp}
                maxHp={player.maxHp}
                mp={player.mp}
                maxMp={player.maxMp}
                dispatch={dispatch}
                setActiveTab={setActiveTab}
              />
            )}

            {/* 상점 - 장비·포션 구매 (마을에서 진입) */}
            {activeTab === 'shop' && (
              <ShopView
                gold={player.gold}
                weapon_tier={player.weapon_tier}
                armor_tier={player.armor_tier}
                accessory_tier={player.accessory_tier}
                dispatch={dispatch}
                onBack={() => setActiveTab('village')}
              />
            )}

            {/* 대장간 - 수리·제작 (마을에서 진입) */}
            {activeTab === 'blacksmith' && (
              <BlacksmithView
                gold={player.gold}
                needsRepair={player.needsRepair}
                inventory={player.inventory}
                weapon_tier={player.weapon_tier}
                armor_tier={player.armor_tier}
                accessory_tier={player.accessory_tier}
                dispatch={dispatch}
                onBack={() => setActiveTab('village')}
              />
            )}

            {/* 던전 - 전투 탐험 */}
            {activeTab === 'dungeon' && (
              <DungeonView
                dungeons={dungeons}
                encyclopedia={encyclopedia}
                player={player}
                unionBonus={unionBonus}
                dispatch={dispatch}
              />
            )}

            {/* 도감 - 처치 몬스터 기록 */}
            {activeTab === 'encyclopedia' && (
              <EncyclopediaView encyclopedia={encyclopedia} />
            )}

            {/* 설정 - 유니온·이름 변경·슬롯 관리 */}
            {activeTab === 'settings' && (
              <SettingsView
                characters={characters}
                activeSlot={activeSlot}
                playerName={player.name}
                unionBonus={unionBonus}
                dispatch={dispatch}
              />
            )}
          </motion.div>
        </AnimatePresence>
      </main>

      {/* 하단 탭 네비게이션 */}
      <nav className="bottom-nav glass-panel">
        {NAV_TABS.map(tab => (
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
