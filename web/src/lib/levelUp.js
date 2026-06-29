/**
 * 레벨업 처리 유틸리티
 *
 * GAIN_EXP 와 COMBAT_WIN 두 액션에서 동일한 레벨업 로직이 사용되므로
 * 중복을 제거하기 위해 공통 함수로 추출했습니다.
 */
import { getUnionBonus } from './unionSystem';

/**
 * 경험치 획득 후 레벨업을 처리합니다. (인플레이스 뮤테이션)
 *
 * 레벨업마다: maxExp × 1.5 성장, maxHp +20, 모든 스탯 +2, 스킬포인트 +1
 *
 * @param {Object} player   - 딥클론된 플레이어 객체 (직접 수정됨)
 * @param {Object} newState - 현재 게임 상태 (유니온 보너스 계산용)
 * @returns {string[]}      - 레벨업 시 추가할 로그 메시지 배열
 */
export const applyLevelUp = (player, newState) => {
  const logs = [];

  while (player.exp >= player.maxExp) {
    player.level        += 1;
    player.exp          -= player.maxExp;
    player.maxExp        = Math.floor(player.maxExp * 1.5); // 경험치 요구량 지수 성장
    player.maxHp        += 20;
    player.hp            = player.maxHp + getUnionBonus(newState).maxHp; // 레벨업 시 HP 전체 회복
    player.stats.str    += 2;
    player.stats.dex    += 2;
    player.stats.int    += 2;
    player.stats.luk    += 2;
    player.skill_points += 1;

    logs.push(`레벨업! 현재 레벨: ${player.level} (+스킬포인트 1)`);
  }

  return logs;
};
