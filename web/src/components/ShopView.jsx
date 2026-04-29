import { motion } from 'framer-motion';
import { ShoppingCart, ArrowLeft, Shield, Sword, Heart } from 'lucide-react';

export default function ShopView({ state, dispatch, onBack }) {
  const shopItems = [
    { id: 'w1', name: '훈련용 목검', desc: 'STR+10, DEX+5', cost: 100, type: 'weapon', tier: 1, stat: { str: 10, dex: 5 } },
    { id: 'w2', name: '강철 세이버', desc: 'STR+40, DEX+20', cost: 500, type: 'weapon', tier: 2, stat: { str: 40, dex: 20 } },
    { id: 'w3', name: '앱솔랩스 소드', desc: 'STR+120, DEX+60', cost: 2500, type: 'weapon', tier: 3, stat: { str: 120, dex: 60 } },
    { id: 'a1', name: '수습 도복', desc: 'HP+200, MP+100', cost: 150, type: 'armor', tier: 1, stat: { maxHp: 200, maxMp: 100 } },
    { id: 'a2', name: '네크로 아머', desc: 'HP+1000, MP+400', cost: 800, type: 'armor', tier: 2, stat: { maxHp: 1000, maxMp: 400 } },
    { id: 'a3', name: '카루타 세트', desc: 'HP+4000, MP+2000', cost: 3000, type: 'armor', tier: 3, stat: { maxHp: 4000, maxMp: 2000 } },
    { id: 'ac1', name: '실버블라썸 링', desc: 'AllStat+5', cost: 300, type: 'accessory', tier: 1, stat: { str: 5, dex: 5, int: 5, luk: 5 } },
    { id: 'ac2', name: '보스 장신구 세트', desc: 'AllStat+20', cost: 1200, type: 'accessory', tier: 2, stat: { str: 20, dex: 20, int: 20, luk: 20 } },
  ];

  const buyItem = (item) => {
    let currentTier = 0;
    if (item.type === 'weapon') currentTier = state.player.weapon_tier || 0;
    if (item.type === 'armor') currentTier = state.player.armor_tier || 0;
    if (item.type === 'accessory') currentTier = state.player.accessory_tier || 0;

    if (item.tier !== currentTier + 1) {
      if (item.tier <= currentTier) {
        dispatch({ type: 'ADD_LOG', payload: '이미 해당 장비(또는 더 높은 장비)를 보유하고 있습니다!' });
      } else {
        dispatch({ type: 'ADD_LOG', payload: '이전 단계의 장비를 먼저 구매해야 합니다!' });
      }
      return;
    }

    dispatch({ type: 'BUY_ITEM', payload: { cost: item.cost, statIncreases: item.stat, newTier: item.tier, type: item.type } });
  };

  const buyPotion = (type) => {
    dispatch({ type: 'BUY_POTION', payload: type });
  };

  return (
    <div className="shop-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: '10px' }}>
        <button className="btn" onClick={onBack} style={{ padding: '8px' }}><ArrowLeft size={20} /></button>
        <h1 className="text-gradient" style={{ fontSize: '2rem', margin: 0 }}>마을 상점</h1>
      </div>
      <p style={{ color: 'var(--text-muted)' }}>필요한 장비와 포션을 구매하세요. 현재 소지 골드: <strong style={{color: '#fbbf24'}}>{state.player.gold}G</strong></p>

      <div className="card-grid">
        {shopItems.filter(item => {
          let currentTier = 0;
          if (item.type === 'weapon') currentTier = state.player.weapon_tier || 0;
          if (item.type === 'armor') currentTier = state.player.armor_tier || 0;
          if (item.type === 'accessory') currentTier = state.player.accessory_tier || 0;
          
          const maxTiers = { weapon: 3, armor: 3, accessory: 2 };
          if (currentTier >= maxTiers[item.type]) {
            return item.tier === maxTiers[item.type];
          }
          return item.tier === currentTier + 1;
        }).map(item => {
          let currentTier = 0;
          if (item.type === 'weapon') currentTier = state.player.weapon_tier || 0;
          if (item.type === 'armor') currentTier = state.player.armor_tier || 0;
          if (item.type === 'accessory') currentTier = state.player.accessory_tier || 0;
          const isBought = currentTier >= item.tier;

          return (
            <motion.div key={item.id} className="stat-card glass-panel" style={{ opacity: isBought ? 0.6 : 1 }}>
              <div className="stat-header">
                <h3>
                  {item.type === 'weapon' && <Sword size={16} className="text-danger" style={{marginRight: '5px'}}/>}
                  {item.type === 'armor' && <Shield size={16} className="text-primary" style={{marginRight: '5px'}}/>}
                  {item.name} {isBought ? '(보유중)' : ''}
                </h3>
              </div>
              <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginTop: '10px' }}>{item.desc}</p>
              <button className="btn btn-primary" style={{ marginTop: '15px' }} onClick={() => buyItem(item)} disabled={isBought}>
                {item.cost}G 구매
              </button>
            </motion.div>
          );
        })}
        
        <motion.div className="stat-card glass-panel">
          <div className="stat-header">
            <h3><Heart size={16} style={{ color: 'red', marginRight: '5px' }} />빨간 포션</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginTop: '10px' }}>HP 50% 회복</p>
          <button className="btn" style={{ marginTop: '15px' }} onClick={() => buyPotion('hp')}>
            30G 구매
          </button>
        </motion.div>
        
        <motion.div className="stat-card glass-panel">
          <div className="stat-header">
            <h3><Heart size={16} style={{ color: 'blue', marginRight: '5px' }} />파란 포션</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginTop: '10px' }}>MP 50% 회복</p>
          <button className="btn" style={{ marginTop: '15px' }} onClick={() => buyPotion('mp')}>
            30G 구매
          </button>
        </motion.div>
      </div>
    </div>
  );
}
