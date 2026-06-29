/**
 * 게임 상태 순수 리듀서
 *
 * 모든 게임 액션 처리가 이 파일에 집중되어 있습니다.
 * - 사이드이펙트(localStorage, 타이머 등) 없음
 * - 항상 새 상태 객체를 반환 (불변성 보장)
 * - 액션별 처리는 switch 블록 내 독립된 스코프({})에서 수행
 */
import { getInitialPlayer } from './playerInit';
import { evaluateJob, getJobSkills } from './jobSystem';
import { getUnionBonus } from './unionSystem';
import { applyLevelUp } from './levelUp';

// ── 내부 헬퍼 ──────────────────────────────────────────────────────────────

/**
 * 로그 배열 앞에 메시지를 삽입하고 최대 50개로 제한합니다.
 * @param {string[]} logs
 * @param {...string} messages
 */
const pushLog = (logs, ...messages) => [...messages, ...logs].slice(0, 50);

/**
 * 전직 조건을 확인하고 조건 충족 시 플레이어를 전직시킵니다. (인플레이스 뮤테이션)
 * @returns {string[]} 전직 관련 로그 메시지 배열 (미전직 시 빈 배열)
 */
const checkJobAdvancement = (player, newState) => {
  // Novice 이며 레벨 10 이상일 때만 전직 평가
  if (player.level < 10 || player.job !== 'Novice') return [];

  const jobResult = evaluateJob(player);
  player.job = jobResult.job;

  // 전직 보너스 스탯 적용
  if (jobResult.bonus.str)   player.stats.str   += jobResult.bonus.str;
  if (jobResult.bonus.dex)   player.stats.dex   += jobResult.bonus.dex;
  if (jobResult.bonus.int)   player.stats.int   += jobResult.bonus.int;
  if (jobResult.bonus.luk)   player.stats.luk   += jobResult.bonus.luk;
  if (jobResult.bonus.maxHp) {
    player.maxHp += jobResult.bonus.maxHp;
    player.hp     = player.maxHp + getUnionBonus(newState).maxHp;
  }
  if (jobResult.bonus.maxMp) {
    player.maxMp += jobResult.bonus.maxMp;
    player.mp     = player.maxMp;
  }

  // 직업 전용 스킬 3종 추가
  player.skills.push(...getJobSkills(jobResult.job));

  return [
    `🎉 [전직 완료!] ${jobResult.desc}`,
    `✨ 10레벨 달성! 새로운 직업 '${jobResult.job}'을(를) 얻었습니다! (전용 스킬 3종 획득)`,
  ];
};

// ── 리듀서 본체 ────────────────────────────────────────────────────────────

/**
 * @param {Object} state - 이전 게임 상태
 * @param {{ type: string, payload?: any }} action
 * @returns {Object} 새 게임 상태
 */
export const gameReducer = (state, action) => {
  // 딥클론: 리듀서는 항상 새 객체를 반환해야 하므로 JSON 직렬화로 복사
  const newState = JSON.parse(JSON.stringify(state));
  const player   = newState.characters[newState.activeSlot];

  switch (action.type) {

    // ── 마을 알바 ───────────────────────────────────────────────────────────
    case 'WORK_JOB': {
      if (player.hp <= 15) {
        newState.logs = pushLog(newState.logs, '체력이 부족하여 알바를 할 수 없습니다. (HP 15 초과 필요)');
        break;
      }

      const roll = Math.floor(Math.random() * 100);
      let gold = 0, hpDelta = 0, msg = '';

      // 확률 분포: 대성공 10% / 성공 50% / 실패 30% / 대실패 10%
      if (roll < 10)      { gold = 40; hpDelta =  10; msg = '✨ [대성공!] 사람들에게 칭찬을 들었습니다! (+40G, HP회복 10)'; }
      else if (roll < 60) { gold = 25; hpDelta =  -5; msg = '✅ [성공] 무난하게 청소를 마쳤습니다. (+25G, HP감소 5)'; }
      else if (roll < 90) { gold = 10; hpDelta = -15; msg = '❓ [실패] 청소가 덜 된 곳이 있습니다. (+10G, HP감소 15)'; }
      else                { gold =  0; hpDelta = -25; msg = '💀 [대실패] 허리를 삐끗했습니다! (+0G, HP감소 25)'; }

      const maxHp = player.maxHp + getUnionBonus(newState).maxHp;
      player.gold += gold;
      player.hp    = Math.max(1, Math.min(maxHp, player.hp + hpDelta));
      player.statistics.timesWorked += 1;

      // 최저 HP% 기록 (광전사 전직 조건 추적)
      const hpPct = (player.hp / maxHp) * 100;
      if (hpPct < player.statistics.lowestHpPercent) player.statistics.lowestHpPercent = hpPct;

      newState.logs = pushLog(newState.logs, msg);
      break;
    }

    // ── 장비 구매 ───────────────────────────────────────────────────────────
    case 'BUY_ITEM': {
      const { cost, statIncreases, newTier, type } = action.payload;
      if (player.gold < cost) {
        newState.logs = pushLog(newState.logs, `장비를 구매할 골드가 부족합니다! (${cost}G 필요)`);
        break;
      }
      player.gold -= cost;
      player.statistics.moneySpent += cost;

      if (statIncreases.str)   player.stats.str   += statIncreases.str;
      if (statIncreases.dex)   player.stats.dex   += statIncreases.dex;
      if (statIncreases.int)   player.stats.int   += statIncreases.int;
      if (statIncreases.luk)   player.stats.luk   += statIncreases.luk;
      if (statIncreases.maxHp) {
        player.maxHp += statIncreases.maxHp;
        player.hp     = player.maxHp + getUnionBonus(newState).maxHp;
      }
      if (statIncreases.maxMp) {
        player.maxMp += statIncreases.maxMp;
        player.mp     = player.maxMp;
      }

      if (type === 'weapon')    player.weapon_tier    = newTier;
      if (type === 'armor')     player.armor_tier     = newTier;
      if (type === 'accessory') player.accessory_tier = newTier;

      newState.logs = pushLog(newState.logs, `상점에서 장비를 구매했습니다! (-${cost}G)`);
      break;
    }

    // ── 포션 구매 ───────────────────────────────────────────────────────────
    case 'BUY_POTION': {
      const POTION_COST = 30;
      if (player.gold < POTION_COST) {
        newState.logs = pushLog(newState.logs, `포션을 살 골드가 부족합니다! (${POTION_COST}G 필요)`);
        break;
      }
      player.gold -= POTION_COST;
      player.statistics.moneySpent  += POTION_COST;
      player.statistics.potionsUsed += 1;

      if (action.payload === 'hp') {
        const maxHp = player.maxHp + getUnionBonus(newState).maxHp;
        player.hp   = Math.min(maxHp, player.hp + Math.floor(player.maxHp / 2));
        newState.logs = pushLog(newState.logs, '빨간 포션을 마셔 체력을 회복했습니다.');
      } else {
        player.mp = Math.min(player.maxMp, player.mp + Math.floor(player.maxMp / 2));
        newState.logs = pushLog(newState.logs, '파란 포션을 마셔 마나를 회복했습니다.');
      }
      break;
    }

    // ── 힌트 구매 (정보 상인) ────────────────────────────────────────────────
    case 'BUY_HINT': {
      const HINT_COST = 500;
      if (player.gold < HINT_COST) {
        newState.logs = pushLog(newState.logs, '정보를 살 골드가 부족합니다! (500G 필요)');
        break;
      }
      player.gold -= HINT_COST;
      const hints = [
        '광전사의 길: 죽음의 문턱(HP 10% 이하)을 경험하고 수많은 몬스터를 벤 강인한 투지(STR)가 필요합니다.',
        '대도의 길: 민첩(DEX)과 행운(LUK)을 갈고닦아 3000G 이상의 부를 축적해야 합니다.',
        '정령술사의 길: 쉴 새 없이 마나포션을 소모하며 지능(INT)의 극의에 달해야 합니다.',
        '전설의 도박사: 아무런 방어구와 무기도 사지 않은 채 자신의 운(LUK 50)만을 믿어야 합니다.',
        '세계의 구원자: 모든 분야(장비 3티어, 모든스탯 30)에 통달해야 전설이 됩니다.',
      ];
      const hint = hints[Math.floor(Math.random() * hints.length)];
      newState.logs = pushLog(newState.logs, `📜 정보 상인: "${hint}"`);
      break;
    }

    // ── 장비 수리 (대장간) ───────────────────────────────────────────────────
    case 'REPAIR_ALL': {
      if (!player.needsRepair) {
        newState.logs = pushLog(newState.logs, '수리할 장비가 없습니다.');
        break;
      }
      const { cost } = action.payload;
      if (player.gold < cost) {
        newState.logs = pushLog(newState.logs, '수리비가 부족합니다!');
        break;
      }
      player.gold       -= cost;
      player.needsRepair = false;
      newState.logs = pushLog(newState.logs, `모든 장비가 완벽하게 수리되었습니다! (-${cost}G)`);
      break;
    }

    // ── 로그 직접 추가 ──────────────────────────────────────────────────────
    case 'ADD_LOG':
      newState.logs = pushLog(newState.logs, action.payload);
      break;

    // ── 골드 획득 ───────────────────────────────────────────────────────────
    case 'EARN_GOLD':
      player.gold += action.payload;
      newState.logs = pushLog(newState.logs, `+${action.payload} Gold 획득!`);
      break;

    // ── 골드 소비 (잔액 부족 시 상태 불변) ──────────────────────────────────
    case 'SPEND_GOLD':
      if (player.gold < action.payload) return state;
      player.gold -= action.payload;
      break;

    // ── 전체 회복 (여관) ─────────────────────────────────────────────────────
    case 'HEAL': {
      const maxHp = player.maxHp + getUnionBonus(newState).maxHp;
      player.hp   = maxHp;
      player.mp   = player.maxMp;
      newState.logs = pushLog(newState.logs, '체력과 마나가 모두 회복되었습니다.');
      break;
    }

    // ── 경험치 획득 ─────────────────────────────────────────────────────────
    case 'GAIN_EXP': {
      player.exp += action.payload;
      const lvLogs    = applyLevelUp(player, newState);
      const jobLogs   = checkJobAdvancement(player, newState);
      newState.logs   = pushLog(newState.logs, ...lvLogs, ...jobLogs);
      break;
    }

    // ── 전투 승리 ───────────────────────────────────────────────────────────
    case 'COMBAT_WIN': {
      const { gold, exp, monsterName, drops } = action.payload;
      player.gold += gold;
      player.exp  += exp;
      player.statistics.monstersKilled += 1;

      // 드롭 아이템을 인벤토리에 반영
      let dropText = '';
      if (drops?.length > 0) {
        dropText = ` (전리품: ${drops.map(d => `${d.name} x${d.count}`).join(', ')})`;
        if (!player.inventory) player.inventory = [];
        drops.forEach(drop => {
          const existing = player.inventory.find(i => i.name === drop.name);
          if (existing) existing.count += drop.count;
          else          player.inventory.push({ name: drop.name, count: drop.count });
        });
      }
      newState.logs = pushLog(newState.logs, `${monsterName} 처치! +${gold}G, +${exp}EXP${dropText}`);

      // 레벨업 및 전직 처리
      const lvLogs  = applyLevelUp(player, newState);
      const jobLogs = checkJobAdvancement(player, newState);
      newState.logs = pushLog(newState.logs, ...lvLogs, ...jobLogs);

      // 도감 처치 수 업데이트
      const entry = newState.encyclopedia.find(e => e.name === monsterName);
      if (entry) entry.kills += 1;
      else       newState.encyclopedia.push({ name: monsterName, kills: 1 });
      break;
    }

    // ── 아이템 제작 (대장간) ─────────────────────────────────────────────────
    case 'CRAFT_ITEM': {
      const { recipe } = action.payload;
      const { cost: craftCost, stat: craftStats, tier: craftTier,
              type: craftType, name: craftName, materials } = recipe;

      if (player.gold < craftCost) {
        newState.logs = pushLog(newState.logs, `제작에 필요한 골드가 부족합니다! (${craftCost}G 필요)`);
        break;
      }

      if (!player.inventory) player.inventory = [];

      // 재료 보유 여부 확인
      const missingMat = materials.find(mat => {
        const inv = player.inventory.find(i => i.name === mat.name);
        return !inv || inv.count < mat.count;
      });
      if (missingMat) {
        newState.logs = pushLog(newState.logs, `재료가 부족하여 [${craftName}]을(를) 제작할 수 없습니다.`);
        break;
      }

      // 선행 장비 티어 확인 (반드시 순서대로 제작)
      const currentTier =
        craftType === 'weapon'    ? (player.weapon_tier    || 0) :
        craftType === 'armor'     ? (player.armor_tier     || 0) :
        craftType === 'accessory' ? (player.accessory_tier || 0) : 0;

      if (craftTier !== currentTier + 1) {
        newState.logs = pushLog(newState.logs, `이전 단계의 장비가 필요하여 [${craftName}]을(를) 제작할 수 없습니다.`);
        break;
      }

      // 골드·재료 차감
      player.gold -= craftCost;
      player.statistics.moneySpent += craftCost;
      materials.forEach(mat => {
        const inv = player.inventory.find(i => i.name === mat.name);
        if (inv) inv.count -= mat.count;
      });
      player.inventory = player.inventory.filter(i => i.count > 0);

      // 스탯·티어 적용
      if (craftStats.str)   player.stats.str   += craftStats.str;
      if (craftStats.dex)   player.stats.dex   += craftStats.dex;
      if (craftStats.int)   player.stats.int   += craftStats.int;
      if (craftStats.luk)   player.stats.luk   += craftStats.luk;
      if (craftStats.maxHp) {
        player.maxHp += craftStats.maxHp;
        player.hp     = player.maxHp + getUnionBonus(newState).maxHp;
      }
      if (craftStats.maxMp) {
        player.maxMp += craftStats.maxMp;
        player.mp     = player.maxMp;
      }

      if (craftType === 'weapon')    player.weapon_tier    = craftTier;
      if (craftType === 'armor')     player.armor_tier     = craftTier;
      if (craftType === 'accessory') player.accessory_tier = craftTier;

      newState.logs = pushLog(newState.logs, `🎉 [제작 성공] ${craftName} 제작을 완료했습니다! (-${craftCost}G)`);
      break;
    }

    // ── 훈련소 무작위 훈련 ──────────────────────────────────────────────────
    case 'TRAIN':
      player.stats[action.payload] += 1;
      player.statistics.moneySpent += 50;
      newState.logs = pushLog(newState.logs, `훈련을 통해 ${action.payload.toUpperCase()}이(가) 1 상승했습니다. (-50G)`);
      break;

    // ── 피해 수령 / 흡혈 회복 (payload 음수 = 회복) ──────────────────────────
    case 'TAKE_DAMAGE': {
      player.hp -= action.payload;
      player.hp  = Math.max(1, player.hp); // 즉사 방지

      // 최저 HP% 기록
      const maxHp = player.maxHp + getUnionBonus(newState).maxHp;
      const hpPct = (player.hp / maxHp) * 100;
      if (hpPct < player.statistics.lowestHpPercent) player.statistics.lowestHpPercent = hpPct;

      // 20% 확률로 장비 손상
      if (Math.random() < 0.2) player.needsRepair = true;
      break;
    }

    // ── 스킬 업그레이드 ─────────────────────────────────────────────────────
    case 'UPGRADE_SKILL': {
      if (player.skill_points <= 0) {
        newState.logs = pushLog(newState.logs, '스킬 포인트가 부족합니다.');
        break;
      }
      const skill = player.skills.find(s => s.id === action.payload);
      if (!skill || skill.level >= skill.maxLevel) {
        newState.logs = pushLog(newState.logs, '이미 마스터한 스킬이거나 스킬을 찾을 수 없습니다.');
        break;
      }
      skill.level         += 1;
      player.skill_points -= 1;
      newState.logs = pushLog(newState.logs, `${skill.name} 스킬이 레벨업 되었습니다! (Lv.${skill.level})`);
      break;
    }

    // ── 이름 변경 ───────────────────────────────────────────────────────────
    case 'CHANGE_NAME':
      player.name = action.payload;
      newState.logs = pushLog(newState.logs, `이름이 ${action.payload}(으)로 변경되었습니다.`);
      break;

    // ── 슬롯 전환 / 신규 생성 ──────────────────────────────────────────────
    case 'SWITCH_SLOT': {
      const slot = action.payload;
      if (!newState.characters[slot]) {
        newState.characters[slot] = getInitialPlayer(`Player ${slot + 1}`);
        newState.logs = pushLog(newState.logs, `새 캐릭터 슬롯(${slot + 1})이 생성되었습니다.`);
      } else {
        newState.logs = pushLog(newState.logs, `캐릭터 슬롯(${slot + 1})으로 전환했습니다.`);
      }
      newState.activeSlot = slot;
      break;
    }

    // ── 슬롯 초기화 (캐릭터 삭제 후 재생성) ───────────────────────────────
    case 'RESET_SLOT': {
      const slot      = action.payload;
      const isCurrent = slot === newState.activeSlot;
      newState.characters[slot] = getInitialPlayer(`Player ${slot + 1}`);
      newState.logs = pushLog(
        newState.logs,
        isCurrent
          ? '현재 플레이 중인 캐릭터가 초기화되었습니다.'
          : `캐릭터 슬롯(${slot + 1})이 초기화되었습니다.`,
      );
      break;
    }

    default:
      // 알 수 없는 액션은 상태를 그대로 반환 (리렌더 방지)
      return state;
  }

  return newState;
};
