/**
 * ShopView - 마을 상점
 *
 * 장비와 포션을 구매할 수 있는 화면입니다.
 * 장비는 티어 순서대로만 구매 가능하며, 현재 티어보다 높은 것은 표시하지 않습니다.
 * 포션(HP/MP)은 항상 구매 가능합니다.
 *
 * 최소 권한 props:
 * - gold:          현재 소지 골드 (구매 가능 여부 표시)
 * - weapon_tier:   현재 무기 티어
 * - armor_tier:    현재 방어구 티어
 * - accessory_tier: 현재 장신구 티어
 * - dispatch, onBack
 */
import { motion } from 'framer-motion';
import { ShoppingCart, ArrowLeft, Shield, Sword, Heart } from 'lucide-react';
import { SHOP_ITEMS } from '../data/shopItems';

/**
 * @param {{
 *   gold: number,
 *   weapon_tier: number,
 *   armor_tier: number,
 *   accessory_tier: number,
 *   dispatch: Function,
 *   onBack: Function
 * }} props
 */
export default function ShopView({ gold, weapon_tier, armor_tier, accessory_tier, dispatch, onBack }) {
  // 현재 장비 티어를 타입별로 빠르게 조회하기 위한 맵
  const currentTiers = { weapon: weapon_tier, armor: armor_tier, accessory: accessory_tier };

  // 최대 상점 판매 티어 (이 이상은 대장간에서 제작)
  const MAX_SHOP_TIERS = { weapon: 3, armor: 3, accessory: 2 };

  /**
   * 장비 구매 처리.
   * 티어 순서 위반 또는 이미 보유 시 경고 로그를 출력합니다.
   */
  const buyItem = (item) => {
    const currentTier = currentTiers[item.type] || 0;

    if (item.tier <= currentTier) {
      dispatch({ type: 'ADD_LOG', payload: '이미 해당 장비(또는 더 높은 장비)를 보유하고 있습니다!' });
      return;
    }
    if (item.tier !== currentTier + 1) {
      dispatch({ type: 'ADD_LOG', payload: '이전 단계의 장비를 먼저 구매해야 합니다!' });
      return;
    }

    dispatch({ type: 'BUY_ITEM', payload: { cost: item.cost, statIncreases: item.stat, newTier: item.tier, type: item.type } });
  };

  const buyPotion = (type) => dispatch({ type: 'BUY_POTION', payload: type });

  // 현재 구매 가능한(= 다음 티어) 장비만 필터링하여 표시
  const visibleItems = SHOP_ITEMS.filter(item => {
    const currentTier = currentTiers[item.type] || 0;
    const maxTier     = MAX_SHOP_TIERS[item.type];
    // 이미 최대 티어 달성 시 마지막 항목만 '보유중'으로 표시
    if (currentTier >= maxTier) return item.tier === maxTier;
    return item.tier === currentTier + 1;
  });

  return (
    <div className="shop-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: '10px' }}>
        <button className="btn" onClick={onBack} style={{ padding: '8px' }}><ArrowLeft size={20} /></button>
        <h1 className="text-gradient" style={{ fontSize: '2rem', margin: 0 }}>마을 상점</h1>
      </div>
      <p style={{ color: 'var(--text-muted)' }}>
        필요한 장비와 포션을 구매하세요. 현재 소지 골드: <strong style={{ color: '#fbbf24' }}>{gold}G</strong>
      </p>

      <div className="card-grid">
        {/* 장비 목록 */}
        {visibleItems.map(item => {
          const isBought = (currentTiers[item.type] || 0) >= item.tier;

          return (
            <motion.div key={item.id} className="stat-card glass-panel" style={{ opacity: isBought ? 0.6 : 1 }}>
              <div className="stat-header">
                <h3>
                  {item.type === 'weapon'    && <Sword  size={16} className="text-danger"  style={{ marginRight: '5px' }} />}
                  {item.type === 'armor'     && <Shield size={16} className="text-primary" style={{ marginRight: '5px' }} />}
                  {item.name} {isBought ? '(보유중)' : ''}
                </h3>
              </div>
              <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginTop: '10px' }}>{item.desc}</p>
              <button
                className={`btn ${gold >= item.cost ? 'btn-primary' : 'btn-danger'}`}
                style={{ marginTop: '15px' }}
                onClick={() => buyItem(item)}
                disabled={isBought}
              >
                {item.cost}G 구매
              </button>
            </motion.div>
          );
        })}

        {/* 빨간 포션 */}
        <motion.div className="stat-card glass-panel">
          <div className="stat-header">
            <h3><Heart size={16} style={{ color: 'red', marginRight: '5px' }} />빨간 포션</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginTop: '10px' }}>HP 50% 회복</p>
          <button
            className={`btn ${gold >= 30 ? '' : 'btn-danger'}`}
            style={{ marginTop: '15px' }}
            onClick={() => buyPotion('hp')}
          >
            30G 구매
          </button>
        </motion.div>

        {/* 파란 포션 */}
        <motion.div className="stat-card glass-panel">
          <div className="stat-header">
            <h3><Heart size={16} style={{ color: 'blue', marginRight: '5px' }} />파란 포션</h3>
          </div>
          <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginTop: '10px' }}>MP 50% 회복</p>
          <button
            className={`btn ${gold >= 30 ? '' : 'btn-danger'}`}
            style={{ marginTop: '15px' }}
            onClick={() => buyPotion('mp')}
          >
            30G 구매
          </button>
        </motion.div>
      </div>
    </div>
  );
}
