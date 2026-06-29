/**
 * BlacksmithView - 대장간 화면
 *
 * 두 가지 기능을 제공합니다:
 * 1. 장비 수리 - 전투 중 손상된 장비를 100G에 수리
 * 2. 아이템 제작 - 던전 몬스터 재료를 조합하여 전설 장비 제작
 *
 * 최소 권한 props:
 * - gold:           현재 소지 골드
 * - needsRepair:    장비 손상 여부
 * - inventory:      보유 재료 목록
 * - weapon_tier / armor_tier / accessory_tier: 현재 장비 티어
 * - dispatch, onBack
 */
import { motion } from 'framer-motion';
import { Hammer, ArrowLeft, Sword, Shield, Gem } from 'lucide-react';
import { CRAFT_RECIPES } from '../data/craftRecipes';

/**
 * @param {{
 *   gold: number,
 *   needsRepair: boolean,
 *   inventory: Array<{ name: string, count: number }>,
 *   weapon_tier: number,
 *   armor_tier: number,
 *   accessory_tier: number,
 *   dispatch: Function,
 *   onBack: Function
 * }} props
 */
export default function BlacksmithView({
  gold, needsRepair, inventory,
  weapon_tier, armor_tier, accessory_tier,
  dispatch, onBack,
}) {
  const currentTiers = { weapon: weapon_tier, armor: armor_tier, accessory: accessory_tier };

  const repairAll = () => dispatch({ type: 'REPAIR_ALL', payload: { cost: 100 } });

  /** 인벤토리에서 특정 재료의 보유 수량을 반환합니다. */
  const getMaterialCount = (name) => inventory?.find(i => i.name === name)?.count ?? 0;

  const handleCraft = (recipe) => dispatch({ type: 'CRAFT_ITEM', payload: { recipe } });

  return (
    <div className="shop-view" style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: '10px' }}>
        <button className="btn" onClick={onBack} style={{ padding: '8px' }}><ArrowLeft size={20} /></button>
        <h1 className="text-gradient" style={{ fontSize: '2rem', margin: 0 }}>대장간</h1>
      </div>
      <p style={{ color: 'var(--text-muted)' }}>
        장비를 수리하거나 강력한 아이템을 제작하세요. (현재 소지 골드: <strong style={{ color: '#fbbf24' }}>{gold}G</strong>)
      </p>

      {/* ── 장비 수리 ─────────────────────────────────────────────────── */}
      <div className="card-grid">
        <motion.div className="stat-card glass-panel" whileHover={{ y: -5 }} style={{ gridColumn: '1 / -1' }}>
          <div className="stat-header">
            <h3><Hammer size={18} style={{ marginRight: '8px', verticalAlign: 'middle' }} />모든 장비 수리</h3>
          </div>
          <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginTop: '10px', flexWrap: 'wrap', gap: '10px' }}>
            <p style={{ fontSize: '0.9rem', color: 'var(--text-muted)', margin: 0 }}>
              {needsRepair
                ? '장비가 손상되었습니다. 모험을 계속하려면 수리가 필요합니다.'
                : '현재 모든 장비의 상태가 완벽합니다.'}
            </p>
            <button
              className={`btn ${gold >= 100 && needsRepair ? 'btn-primary' : 'btn-danger'}`}
              style={{ minWidth: '120px' }}
              onClick={repairAll}
              disabled={!needsRepair}
            >
              100G 수리
            </button>
          </div>
        </motion.div>
      </div>

      {/* ── 제작 레시피 ────────────────────────────────────────────────── */}
      <h2 className="text-gradient" style={{ fontSize: '1.5rem', marginTop: '10px' }}>장비 제작 레시피</h2>
      <div className="card-grid">
        {CRAFT_RECIPES.map(recipe => {
          const currentTier = currentTiers[recipe.type] || 0;
          const isAlreadyCrafted  = currentTier >= recipe.tier;
          const isPrerequisiteMet = currentTier === recipe.tier - 1;

          // 재료 보유 현황
          const matCountInfo    = recipe.materials.map(mat => ({
            ...mat,
            has:      getMaterialCount(mat.name),
            isEnough: getMaterialCount(mat.name) >= mat.count,
          }));
          const hasEnoughMaterials = matCountInfo.every(m => m.isEnough);
          const hasEnoughGold      = gold >= recipe.cost;

          // 버튼 텍스트·클래스·비활성 여부 결정
          let buttonText = `${recipe.cost}G 제작`;
          let buttonClass = 'btn-primary';
          let isDisabled  = false;

          if (isAlreadyCrafted)      { buttonText = '보유 중';            isDisabled = true; buttonClass = 'btn'; }
          else if (!isPrerequisiteMet) { buttonText = '이전 단계 장비 필요'; isDisabled = true; buttonClass = 'btn-danger'; }
          else if (!hasEnoughGold)   { buttonText = '골드 부족';          isDisabled = true; buttonClass = 'btn-danger'; }
          else if (!hasEnoughMaterials) { buttonText = '재료 부족';       isDisabled = true; buttonClass = 'btn-danger'; }

          return (
            <motion.div
              key={recipe.id}
              className="stat-card glass-panel"
              style={{ opacity: isAlreadyCrafted ? 0.6 : 1, display: 'flex', flexDirection: 'column', justifyContent: 'space-between', gap: '15px' }}
              whileHover={{ y: -5 }}
            >
              <div>
                <div className="stat-header" style={{ marginBottom: '10px' }}>
                  <h3 style={{ display: 'flex', alignItems: 'center', gap: '6px', fontSize: '1.1rem' }}>
                    {recipe.type === 'weapon'    && <Sword  size={16} className="text-danger"    />}
                    {recipe.type === 'armor'     && <Shield size={16} className="text-primary"   />}
                    {recipe.type === 'accessory' && <Gem    size={16} className="text-secondary" />}
                    {recipe.name}
                  </h3>
                  <span style={{ fontSize: '0.75rem', color: 'var(--text-muted)' }}>Tier {recipe.tier}</span>
                </div>
                <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)', marginBottom: '12px' }}>{recipe.desc}</p>

                {/* 재료 보유 현황 표 */}
                <div style={{ background: 'rgba(0,0,0,0.2)', padding: '10px', borderRadius: '8px', display: 'flex', flexDirection: 'column', gap: '6px' }}>
                  <div style={{ fontSize: '0.8rem', fontWeight: 'bold', color: 'var(--text-muted)', borderBottom: '1px solid var(--glass-border)', paddingBottom: '4px', marginBottom: '4px' }}>
                    필요 재료:
                  </div>
                  {matCountInfo.map(mat => (
                    <div
                      key={mat.name}
                      style={{ fontSize: '0.8rem', display: 'flex', justifyContent: 'space-between', color: mat.isEnough ? 'var(--accent)' : 'var(--danger)' }}
                    >
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
                disabled={isDisabled}
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
