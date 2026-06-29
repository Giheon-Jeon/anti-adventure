/**
 * useGameState 훅
 *
 * 게임 전체 상태를 관리하는 메인 훅입니다.
 * 이 파일의 역할:
 * 1. 초기 상태 로드 (localStorage → 이전 세이브 복원 or 신규 생성)
 * 2. 상태 변경 시 localStorage 자동 저장
 * 3. dispatch 함수 노출 (실제 로직은 gameReducer.js 에 위임)
 *
 * 로직 분리 구조:
 * - 게임 로직:   src/lib/gameReducer.js
 * - 직업 시스템: src/lib/jobSystem.js
 * - 유니온 보너스: src/lib/unionSystem.js
 * - 레벨업:      src/lib/levelUp.js
 * - 전투 계산:   src/lib/combatUtils.js
 * - 초기 상태:   src/lib/playerInit.js
 */
import { useState, useCallback, useEffect } from 'react';
import { getInitialPlayer, initialState } from '../lib/playerInit';
import { getUnionBonus } from '../lib/unionSystem';
import { gameReducer } from '../lib/gameReducer';

// 저장 키는 상수로 관리 (오타 방지)
const SAVE_KEY = 'anti-adventure-save';

/**
 * 로컬스토리지에서 세이브 데이터를 불러오고 마이그레이션을 적용합니다.
 * useState의 initializer 함수로 사용되어 최초 렌더링에만 실행됩니다.
 *
 * @returns {Object} 게임 상태
 */
const loadSave = () => {
  const saved = localStorage.getItem(SAVE_KEY);
  if (!saved) return initialState;

  try {
    const parsed = JSON.parse(saved);
    return migrate(parsed);
  } catch {
    // 저장 데이터 파싱 실패 시 초기 상태로 복구
    return initialState;
  }
};

/**
 * 구버전 세이브 데이터를 현재 스키마로 마이그레이션합니다.
 *
 * 마이그레이션 이력:
 * - 단일 플레이어(player) → 멀티 슬롯(characters[]) 구조 전환
 * - statistics 필드 누락 슬롯 보완
 * - inventory 필드 누락 슬롯 보완
 * - dungeons 배열은 항상 최신 값으로 덮어씌움
 *
 * @param {Object} parsed - JSON.parse 결과
 * @returns {Object} 마이그레이션된 상태
 */
const migrate = (parsed) => {
  if (!parsed.characters) {
    // v1 → v2: 단일 player 객체를 3슬롯 구조로 변환
    const oldPlayer = parsed.player || getInitialPlayer('Player 1');
    if (!oldPlayer.skills)           oldPlayer.skills       = getInitialPlayer().skills;
    if (oldPlayer.skill_points == null) oldPlayer.skill_points = 0;
    if (!oldPlayer.statistics)       oldPlayer.statistics   = getInitialPlayer().statistics;
    if (!oldPlayer.inventory)        oldPlayer.inventory    = [];

    parsed.characters = [oldPlayer, null, null];
    parsed.activeSlot = 0;
  } else {
    // v2 이상: 슬롯별 누락 필드 보완 (신규 필드 추가 시 기존 세이브 호환)
    parsed.characters.forEach(char => {
      if (!char) return;
      if (!char.statistics) char.statistics = getInitialPlayer().statistics;
      if (!char.inventory)  char.inventory  = [];
    });
  }

  // 던전 목록은 항상 코드상 최신 값 사용 (잠금 해제 등 동기화)
  parsed.dungeons = initialState.dungeons;
  return parsed;
};

// ── 공개 훅 ────────────────────────────────────────────────────────────────

/**
 * 게임 상태와 dispatch 함수를 반환하는 커스텀 훅.
 *
 * @returns {{
 *   gameState: Object & { player: Object, unionBonus: Object },
 *   dispatch: (action: { type: string, payload?: any }) => void
 * }}
 */
export function useGameState() {
  const [gameState, setGameState] = useState(loadSave);

  // 상태 변경 시마다 localStorage에 저장
  useEffect(() => {
    localStorage.setItem(SAVE_KEY, JSON.stringify(gameState));
  }, [gameState]);

  // gameReducer는 순수 함수이므로 useCallback 의존성 배열은 비어있어도 안전
  const dispatch = useCallback((action) => {
    setGameState(prev => gameReducer(prev, action));
  }, []);

  // 매 렌더마다 유니온 보너스 재계산 (비활성 슬롯 변경 감지)
  const unionBonus = getUnionBonus(gameState);

  return {
    gameState: {
      ...gameState,
      player: gameState.characters[gameState.activeSlot], // 현재 활성 캐릭터 단축키
      unionBonus,
    },
    dispatch,
  };
}

// 하위 호환성을 위해 유니온 보너스 계산 함수도 재익스포트
export { getUnionBonus };

// 직업 스킬 조회 함수도 재익스포트 (CharacterView 등에서 사용 가능)
export { getJobSkills } from '../lib/jobSystem';
