import { motion } from 'framer-motion';
import { Hammer, ArrowLeft, Sword, Shield, Gem } from 'lucide-react';

const craftRecipes = [
  {
    id: 'c_w4_1',
    name: '용살자의 대검',
    desc: 'STR+300, DEX+100 (전설 무기)',
    type: 'weapon',
    tier: 4,
    cost: 5000,
    stat: { str: 300, dex: 100 },
    materials: [
      { name: '용의 비늘', count: 5 },
      { name: '드래곤의 심장', count: 1 },
      { name: '나뭇가지', count: 10 }
    ]
  },
  {
    id: 'c_w4_2',
    name: '사신의 낫',
    desc: 'STR+200, LUK+150 (전설 무기)',
    type: 'weapon',
    tier: 4,
    cost: 4000,
    stat: { str: 200, luk: 150 },
    materials: [
      { name: '해골 조각', count: 10 },
      { name: '유령의 눈물', count: 5 },
      { name: '심연의 정수', count: 3 }
    ]
  },
  {
    id: 'c_a4_1',
    name: '드래곤 스케일 아머',
    desc: 'HP+10000, MP+3000 (전설 방어구)',
    type: 'armor',
    tier: 4,
    cost: 5000,
    stat: { maxHp: 10000, maxMp: 3000 },
    materials: [
      { name: '용의 비늘', count: 8 },
      { name: '이블아이의 꼬리', count: 10 },
      { name: '와일드보어의 송곳니', count: 10 }
    ]
  },
  {
    id: 'c_a4_2',
    name: '심연의 마법 로브',
    desc: 'HP+6000, MP+6000 (전설 방어구)',
    type: 'armor',
    tier: 4,
    cost: 4500,
    stat: { maxHp: 6000, maxMp: 6000 },
    materials: [
      { name: '심연의 정수', count: 5 },
      { name: '마계의 돌', count: 5 },
      { name: '부적', count: 10 }
    ]
  },
  {
    id: 'c_ac3_1',
    name: '여명의 귀걸이',
    desc: 'AllStat+50 (고대 장신구)',
    type: 'accessory',
    tier: 3,
    cost: 3000,
    stat: { str: 50, dex: 50, int: 50, luk: 50 },
    materials: [
      { name: '유령의 눈물', count: 8 },
      { name: '루팡의 바나나', count: 15 },
      { name: '슬라임 방울', count: 20 }
    ]
  },
  {
    id: 'c_ac3_2',
    name: '드림 브레이커 링',
    desc: 'AllStat+80 (고대 장신구)',
    type: 'accessory',
    tier: 3,
    cost: 6000,
    stat: { str: 80, dex: 80, int: 80, luk: 80 },
    materials: [
      { name: '드래곤의 심장', count: 2 },
      { name: '심연의 정수', count: 5 },
      { name: '마계의 돌', count: 8 }
    ]
  }
];

export default function BlacksmithView({ state, dispatch, onBack }) {
  const { player } = state;
  const repairAll = () => {
    dispatch({ type: 'REPAIR_ALL', payload: { cost: 100 } });
  };

  const getMaterialCount = (name) => {
    const item = player.inventory?.find(i => i.name === name);
    return item ? item.count : 0;
  };

  const handleCraft = (recipe) => {
    dispatch({ type: 'CRAFT_ITEM', payload: { recipe } });
  };

  return (
    <div className="shop-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: '10px' }}>
        <button className="btn" onClick={onBack} style={{ padding: '8px' }}><ArrowLeft size={20} /></button>
        <h1 className="text-gradient" style={{ fontSize: '2rem', margin: 0 }}>대장간</h1>
      </div>
      <p style={{ color: 'var(--text-muted)' }}>장비를 수리하거나 강력한 아이템을 제작하세요. (현재 소지 골드: <strong style={{color: '#fbbf24'}}>{player.gold}G</strong>)</p>

      <div className="card-grid">
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }} style={{ gridColumn: '1 / -1' }}>
          <div className="stat-header">
            <h3><Hammer size={18} style={{marginRight: '8px', verticalAlign: 'middle'}}/>모든 장비 수리</h3>
          </div>
          <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginTop: '10px', flexWrap: 'wrap', gap: '10px' }}>
            <p style={{ fontSize: '0.9rem', color: 'var(--text-muted)', margin: 0 }}>
              {player.needsRepair ? '장비가 손상되었습니다. 모험을 계속하려면 수리가 필요합니다.' : '현재 모든 장비의 상태가 완벽합니다.'}
            </p>
            <button 
              className={`btn ${player.gold >= 100 && player.needsRepair ? 'btn-primary' : 'btn-danger'}`}
              style={{ minWidth: '120px' }}
              onClick={repairAll}
              disabled={!player.needsRepair}
            >
              100G 수리
            </button>
          </div>
        </motion.div>
      </div>

      <h2 className="text-gradient" style={{ fontSize: '1.5rem', marginTop: '10px' }}>장비 제작 레시피</h2>
      <div className="card-grid">
        {craftRecipes.map(recipe => {
          const matCountInfo = recipe.materials.map(mat => {
            const has = getMaterialCount(mat.name);
            return { ...mat, has, isEnough: has >= mat.count };
          });

          const hasEnoughMaterials = matCountInfo.every(m => m.isEnough);
          const hasEnoughGold = player.gold >= recipe.cost;
          
          let currentTier = 0;
          if (recipe.type === 'weapon') currentTier = player.weapon_tier || 0;
          if (recipe.type === 'armor') currentTier = player.armor_tier || 0;
          if (recipe.type === 'accessory') currentTier = player.accessory_tier || 0;

          const isAlreadyCrafted = currentTier >= recipe.tier;
          const isPrerequisiteMet = currentTier === recipe.tier - 1;
          
          let buttonText = `${recipe.cost}G 제작`;
          let buttonClass = 'btn-primary';
          let isButtonDisabled = false;

          if (isAlreadyCrafted) {
            buttonText = '보유 중';
            isButtonDisabled = true;
            buttonClass = 'btn';
          } else if (!isPrerequisiteMet) {
            buttonText = `이전 단계 장비 필요`;
            isButtonDisabled = true;
            buttonClass = 'btn-danger';
          } else if (!hasEnoughGold) {
            buttonText = '골드 부족';
            isButtonDisabled = true;
            buttonClass = 'btn-danger';
          } else if (!hasEnoughMaterials) {
            buttonText = '재료 부족';
            isButtonDisabled = true;
            buttonClass = 'btn-danger';
          }

          return (
            <motion.div key={recipe.id} className="stat-card glass-panel" style={{ opacity: isAlreadyCrafted ? 0.6 : 1, display: 'flex', flexDirection: 'column', justifyContent: 'space-between', gap: '15px' }} whileHover={{ y: -5 }}>
              <div>
                <div className="stat-header" style={{ marginBottom: '10px' }}>
                  <h3 style={{ display: 'flex', alignItems: 'center', gap: '6px', fontSize: '1.1rem' }}>
                    {recipe.type === 'weapon' && <Sword size={16} className="text-danger" />}
                    {recipe.type === 'armor' && <Shield size={16} className="text-primary" />}
                    {recipe.type === 'accessory' && <Gem size={16} className="text-secondary" />}
                    {recipe.name}
                  </h3>
                  <span style={{ fontSize: '0.75rem', color: 'var(--text-muted)' }}>Tier {recipe.tier}</span>
                </div>
                <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginBottom: '12px' }}>{recipe.desc}</p>
                
                {/* Materials Required */}
                <div style={{ background: 'rgba(0,0,0,0.2)', padding: '10px', borderRadius: '8px', display: 'flex', flexDirection: 'column', gap: '6px' }}>
                  <div style={{ fontSize: '0.8rem', fontWeight: 'bold', color: 'var(--text-muted)', borderBottom: '1px solid var(--glass-border)', paddingBottom: '4px', marginBottom: '4px' }}>필요 재료:</div>
                  {matCountInfo.map(mat => (
                    <div key={mat.name} style={{ fontSize: '0.8rem', display: 'flex', justifyContent: 'space-between', color: mat.isEnough ? 'var(--accent)' : 'var(--danger)' }}>
                      <span>• {mat.name} ({mat.count}개)</span>
                      <span>보유: {mat.has}개</span>
                    </div>
                  ))}
                </div>
              </div>

              <button 
                className={`btn ${buttonClass}`} 
                style={{ width: '100%' }} 
                onClick={() => handleCraft(recipe)}
                disabled={isButtonDisabled}
              >
                {buttonText}
              </button>
            </motion.div>
          );
        })}
      </div>
    </div>
  );
}
