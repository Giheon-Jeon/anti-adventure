# anti-adventure 🛡️

기존 C언어 기반의 CLI 환경에서 동작하던 TRPG 로직을 **React 기반 웹 애플리케이션으로 전면 전환(Web Transition)**한 프로젝트입니다.  
사용자의 활동과 선택에 따라 캐릭터가 성장하며, 16종의 다양한 전직 및 등급제 이벤트를 통해 다채로운 모험을 경험할 수 있습니다. 이제 웹 브라우저를 통해 더욱 직관적이고 쾌적한 인터페이스로 게임을 즐길 수 있습니다.

---

## 🚀 주요 기능 (Main Features)

### 1. 전직 및 성장 시스템
- **16종의 직업군**: 기초 스탯 분배에 따라 Single, Hybrid, Elite, 그리고 전설적인 **Legendary(Avatar)** 직업까지 전직이 가능합니다.
- **자동 스탯 성장**: 무작위 성장 알고리즘과 특정 활동(훈련, 지식 습득 등)을 연동한 스탯 강화 시스템을 갖추고 있습니다.

### 2. 직관적인 웹 UI 시스템 (Web Transition)
- **React 기반 뷰 컴포넌트**: 웹 브라우저 상에서 캐릭터 상태, 인벤토리, 던전, 도감 등을 직관적으로 탐색할 수 있습니다.
- **실시간 상태 연동**: React의 상태 관리를 활용하여 전투 및 성장 결과가 화면에 즉각적으로 반영됩니다.
- **접근성 향상**: 기존 터미널 명령어나 엔터 키 기반 조작에서 벗어나 클릭 한 번으로 쾌적하게 게임을 플레이할 수 있습니다.

### 3. 경제 및 강화된 보상 체계
- **내구도 관리**: 모든 장비에는 내구도가 존재하며, 전투 중 감소합니다. 마을의 **대장간**에서 이를 수리하고 관리해야 합니다.
- **빠른 성장 (Fast Pacing)**: 경험치 요구량을 최적화하고 몬스터 처치(골드 획득 공식 상향) 및 마을 알바 보상을 대폭 상향하여, 심한 반복 플레이(노가다) 없이도 시원시원하고 자연스러운 경제 성장을 경험할 수 있습니다.
- **등급제 이벤트**: Common부터 Legendary까지 다양한 등급의 랜덤 인카운터를 통해 아이템과 골드를 획득합니다.

### 4. 전리품 & 제작 시스템 (Refactored)
- **전리품 자동 획득**: 몬스터를 처치하면 **해당 몬스터의 전리품**이 인벤토리에 자동으로 추가됩니다.
- **전리품 기반 제작**: 수집한 전리품(달팽이 껍질, 슬라임의 핵, 오크의 이빨 등)을 **대장간에서 조합**하여 강력한 제작 장비를 만들 수 있습니다.
- **4단계 제작 성장**: Tier 1(90% 성공) → Tier 4(30% 성공)까지 점진적으로 어려워지는 제작 난이도와 실패 시 재료 손실 리스크가 존재합니다.

### 5. 몬스터 도감 시스템 (Refactored)
- **자동 등록**: 전투에서 처치한 몬스터가 **도감에 자동으로 기록**되며, 누적 처치 수가 추적됩니다.
- **연구 등급 시스템**: 처치 수에 따라 연구 Lv.1 → Lv.3까지 성장하며, 각 단계마다 보상이 발생합니다.
  - **Lv.1**: 몬스터의 HP, 방어율 정보 공개
  - **Lv.2** (10회 처치): 해당 몬스터 상대 **데미지 +20%** 영구 적용
  - **Lv.3** (30회 처치): 보스 약점 노출 / 일반 몬스터 자동 사냥 해금

### 6. 어빌리티 & 연속 공격 (Refactored)
- **3슬롯 잠재 능력**: 신비한 재단에서 어빌리티를 리롤하여 NORMAL~LEGENDARY 등급의 능력을 획득합니다.
- **연속 공격 (Double Attack)**: DOUBLE_ATTACK 어빌리티가 발동하면 해당 턴에 **데미지가 2배**로 적용됩니다.
- **잠금 시스템**: 원하는 어빌리티를 잠금(Lock)하여 리롤 시에도 보존할 수 있습니다.

### 7. 프론트엔드 아키텍처 (Vite + React)
- **SPA 기반 게임 플레이**: Vite와 React를 통해 구축된 Single Page Application 환경에서 매끄러운 뷰 전환(마을, 던전, 캐릭터 창 등)을 제공합니다.
- **풍부한 시각 효과**: 기존 터미널에서 표현할 수 없었던 다양한 UI 컴포넌트와 상호작용을 지원합니다.

### 8. 파일 저장 및 불러오기 (Save System)
- **영구 보존 세이브 (CSV)**: 캐릭터의 능력치, 인벤토리, 장비, 몬스터 도감, 학습 스킬 등 모든 게임 진행 데이터를 CSV 형식으로 안전하게 저장합니다.
- **원클릭 로드**: 게임 진입 시 존재하는 세이브 파일(유저 이름) 목록에서 간편하게 이전 모험을 이어갈 수 있습니다.

---

## 🛠 기술 스택 및 선정 이유 (Tech Stack & Rationale)

### Web Application (Frontend)
![React](https://img.shields.io/badge/React-61DAFB?style=for-the-badge&logo=react&logoColor=white)
![Vite](https://img.shields.io/badge/Vite-646CFF?style=for-the-badge&logo=vite&logoColor=white)

- **Vite & React**: 레거시 CLI 환경에서 벗어나 사용자와 상호작용 가능한 동적이고 직관적인 웹 인터페이스(SPA)를 구축하기 위해 도입했습니다.

### Legacy Core Logic
![C Language](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white)

- **C Language**: 기초적인 게임 로직과 데이터 구조를 다루었던 초기 프로젝트의 핵심 기반입니다.

### Dev Environment
![ESLint](https://img.shields.io/badge/ESLint-4B32C3?style=for-the-badge&logo=eslint&logoColor=white)
![Prettier](https://img.shields.io/badge/Prettier-F7B93E?style=for-the-badge&logo=prettier&logoColor=white)

- **ESLint & Prettier**: 코드 컨벤션을 통일하고 잠재적 에러를 사전 방지하여 프로젝트의 유지보수성을 확보합니다.

---

## 🏗 아키텍처 및 폴더 구조 (Architecture)

유지보수성과 확장성을 고려한 **기능 단위(Feature-based)** 모듈화 구조를 채택했습니다.

```text
anti-adventure/
├── web/                    # 🌐 Web Client (React)
│   ├── src/                # 프론트엔드 소스 코드 (.jsx, .css)
│   │   ├── components/     # UI 뷰 컴포넌트 (Village, Dungeon, Character 등)
│   │   ├── hooks/          # 커스텀 훅 (useGameState 등)
│   │   ├── App.jsx         # 메인 라우팅 및 뷰 전환 
│   │   └── index.css       # 글로벌 스타일링
│   ├── package.json        # 의존성 관리
│   └── vite.config.js      # 빌드 설정
├── trpg/                   # 🏰 Legacy C Engine
│   ├── src/                # 핵심 게임 로직 소스 코드 (.c)
│   ├── include/            # 데이터 구조 정의 (.h)
│   └── data/               # 게임 데이터 (monsters, dungeons)
├── dashboard/              # 📊 (Deprecated) Legacy Dashboard
└── README.md               # 프로젝트 메인 문서
```

---

## ⚙️ 시작하기 (Getting Started)

### 사전 준비
- Node.js 설치

### 웹 클라이언트 실행 (Web Version)
CLI 버전에서 웹 기반으로 완전히 전환되었습니다. 아래 명령어를 통해 개발 서버를 실행할 수 있습니다.
1. `web` 폴더로 이동합니다.
   ```bash
   cd web
   ```
2. 패키지를 설치하고 개발 서버를 구동합니다.
   ```bash
   npm install
   npm run dev
   ```
3. 브라우저에서 `http://localhost:5173`으로 접속하여 게임을 즐기세요!

---

## 🎮 게임 흐름 (Game Flow)

1. **캐릭터 생성**: 초기 능력치가 무작위로 부여됩니다.
2. **마을 활동**: 대장간에서 장비를 수리하거나, 훈련과 알바로 성장합니다.
3. **모험 & 사냥**: 5개의 던전(초심자의 들판 ~ 검은 용의 둥지)에서 적을 물리쳐 **전리품을 수집**하고 **도감을 채웁니다**.
4. **장비 제작**: 전리품을 모아 대장간에서 특수 장비를 제작합니다.
5. **전직 & 연구**: 레벨 10에 자동 전직이 진행되고, 도감 연구 단계가 오르면 전투 효율이 극대화됩니다.
6. **목표**: 최상위 전직인 '전설적인 아바타'에 도달하세요!

---

## 🔧 최근 리팩터링 이력 (v3.0 Web Transition & Features)

가장 최근 업데이트를 통해 기존 CLI 환경에서 모던 웹 애플리케이션으로 전면 전환되었습니다.

### Web 버전 전면 전환 (React + Vite)
- 기존 터미널 기반(`trpg/`)의 로직을 바탕으로, 웹 기반 SPA(단일 페이지 애플리케이션) 인터페이스로 기능을 이전 및 개편했습니다.
- `web/` 디렉터리에 새로운 React 프로젝트를 구성하고, `CharacterView`, `DungeonView`, `VillageView` 등 주요 화면을 컴포넌트화했습니다.
- `useGameState` 커스텀 훅을 통한 전역 상태 관리 구조를 도입했습니다.
- 더 이상 사용되지 않는 CLI용 빌드 스크립트(`build.bat`, `Makefile`, `Dockerfile` 등)를 정리하여 프로젝트를 경량화했습니다.

---

### v2.0 이전 리팩터링 및 기능 확장 이력 (Legacy)
과거 C언어 기반 개발 단계에서 개선된 사항들입니다.

### 신규 기능 추가 및 밸런스 조정
- **세이브/로드 시스템 완비**: CSV 포맷을 사용한 영구 보존 세이브 및 로드 연동 완료
- **경제 밸런스 완화**: 마을 알바 보상 최대 상향(20G -> 40G) 및 사냥 기댓값 조정을 통한 파밍 피로도 완화

### 구조 개선
- **헤더 모듈화**: `Skill`/`SkillType` 타입을 `skill.h`로 분리하여 `character.h`의 비대화 해소
- **함수 분리**: 130줄짜리 `check_level_up()`을 `try_job_advancement()` + `distribute_stat_points()`로 분리
- **중복 제거**: 경험치 공식 3곳 중복 → `get_required_exp()` 헬퍼로 통일, 직업명 배열 중복 제거

### 미구현 시스템 연동
| 시스템 | Before | After |
|--------|--------|-------|
| 전리품 획득 | 몬스터 loot 정의만 존재 | 전투 승리 시 인벤토리에 자동 추가 |
| 도감 등록 | 도감이 항상 비어있음 | 처치 시 자동 등록 + kill_count 누적 |
| 도감 연구 보너스 | 텍스트로만 표시 | 실제 전투에 +20% 데미지 적용 |
| 연속 공격 | 어빌리티만 존재, 효과 없음 | 발동 시 데미지 2배 적용 |
| 던전 선택 메뉴 | 3개 하드코딩 | 5개 던전 동적 생성 + 진입 가능 여부 표시 |

### 데드코드 제거
- `apply_skill_effects()`: 빈 함수 — 제거
- `select_level_up_skill()`: 호출처 없는 안내 메시지 함수 — 제거
- `get_ability_name()`: 선언만 있고 구현 없음 — 선언 제거
- `ultimate_skill`, `has_ultimate`: 미사용 필드 — Player 구조체에서 제거

### UI 일관성 통일
- 모든 화면을 **80칸 박스 레이아웃**으로 표준화
- `show_status()`, `show_inventory()`, 대장간 메뉴 등 레거시 UI 갱신
- 상점 구매 후 **전투력(CP) 즉시 갱신**, 방어구 구매 시 **HP/MP 전회복** 적용
- 빌드 경고 수준 강화 (`-Wall -Wextra`)
