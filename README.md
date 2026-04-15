# anti-adventure 🛡️

Windows 터미널 환경에서 즐기는 **데이터 기반 C언어 TRPG** 프로젝트입니다.  
사용자의 활동과 선택에 따라 캐릭터가 성장하며, 16종의 다양한 전직 및 등급제 이벤트를 통해 다채로운 모험을 경험할 수 있습니다. 최근에는 게임 데이터를 시각화하는 **웹 대시보드**와 **몬스터 도감 시스템**이 추가되었습니다.

---

## 🚀 주요 기능 (Main Features)

### 1. 전직 및 성장 시스템
- **16종의 직업군**: 기초 스탯 분배에 따라 Single, Hybrid, Elite, 그리고 전설적인 **Legendary(Avatar)** 직업까지 전직이 가능합니다.
- **자동 스탯 성장**: 무작위 성장 알고리즘과 특정 활동(훈련, 지식 습득 등)을 연동한 스탯 강화 시스템을 갖추고 있습니다.

### 2. 전투 및 UI 시스템 (Improved)
- **Pixel-Perfect TUI**: 한글/영문 가변 너비를 계산하는 정렬 알고리즘을 도입하여, 깨짐 없는 박스 레이아웃과 중앙 정렬을 구현했습니다.
- **엔터 기반 전투 (One-Tap Combat)**: 불필요한 선택지를 제거하고 **엔터키 하나로** 공격과 턴 진행이 가능한 쾌적한 전투 흐름을 제공합니다.
- **비주얼 바 (Visual Indicators)**: 캐릭터와 몬스터의 **HP Bar**, 그리고 성장을 한눈에 보여주는 **EXP Bar**를 도입하여 시각적 정보를 강화했습니다.
- **전투 결과 리포트**: 처치한 적, 획득 골드, 경험치를 상자 형태의 리포트로 상세히 제공합니다.

### 3. 경제 및 강화된 보상 체계
- **내구도 관리**: 모든 장비에는 내구도가 존재하며, 전투 중 감소합니다. 마을의 **대장간**에서 이를 수리하고 관리해야 합니다.
- **빠른 성장 (Fast Pacing)**: 경험치 요구량을 최적화하고 몬스터 처치 보상을 대폭 상향(평균 3배)하여 시원시원한 성장감을 느낄 수 있습니다.
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

### 7. 데이터 대시보드 (Visual Studio)
- **Vite + React 기반**: C언어로 구현된 게임 데이터를 시각적으로 확인할 수 있는 웹 대시보드를 제공합니다.
- **몬스터 & 던전 데이터**: 게임 내 존재하거나 발견한 데이터를 한눈에 파악할 수 있습니다.

---

## 🛠 기술 스택 및 선정 이유 (Tech Stack & Rationale)

### Core Integration
![C Language](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white)
![Windows CMD](https://img.shields.io/badge/Windows_CMD-000000?style=for-the-badge&logo=windows-terminal&logoColor=white)
![Batch Script](https://img.shields.io/badge/Batch_Script-4EAA25?style=for-the-badge&logo=windows&logoColor=white)

- **C Language**: 포인터, 구조체 등 메모리 관리 기초와 절차적 프로그래밍의 핵심 로직을 직접 구현하기 위해 선정되었습니다.
- **Windows CMD**: ANSI Escape Code를 활용하여 별도의 그래픽 라이브러리 없이도 다채로운 텍스트 UI(TUI)를 구현합니다.
- **Batch Script**: 멀티파일 컴파일 및 빌드 프로세스를 자동화하여 개발 생산성을 높였습니다.

### Modern Dashboard (Frontend)
![React](https://img.shields.io/badge/React-61DAFB?style=for-the-badge&logo=react&logoColor=white)
![Vite](https://img.shields.io/badge/Vite-646CFF?style=for-the-badge&logo=vite&logoColor=white)

- **Vite & React**: 빠르고 가벼운 개발 환경에서 TUI 게임의 정적인 데이터를 동적이고 직관적인 웹 UI로 시각화하기 위해 도입했습니다.

### Dev Environment
![ESLint](https://img.shields.io/badge/ESLint-4B32C3?style=for-the-badge&logo=eslint&logoColor=white)
![Prettier](https://img.shields.io/badge/Prettier-F7B93E?style=for-the-badge&logo=prettier&logoColor=white)

- **ESLint & Prettier**: 코드 컨벤션을 통일하고 잠재적 에러를 사전 방지하여 프로젝트의 유지보수성을 확보합니다.

---

## 🏗 아키텍처 및 폴더 구조 (Architecture)

유지보수성과 확장성을 고려한 **기능 단위(Feature-based)** 모듈화 구조를 채택했습니다.

```text
anti-adventure/
├── trpg/                   # 🏰 TRPG Core Project
│   ├── src/                # 핵심 로직 소스 코드 (.c)
│   │   ├── main.c          # 프로그램 진입점 및 메인 루프
│   │   ├── character.c     # 스탯 성장, 전직, 사망 패널티, 전투력 계산
│   │   ├── combat.c        # 전투 엔진, 야추 시스템, 전리품/도감 연동
│   │   ├── skill.c         # 스킬 DB 관리, 스킬 트리, 콤보 스킬
│   │   ├── ability.c       # 어빌리티 시스템 (잠재 능력)
│   │   ├── shop.c          # 상점 (장비 업그레이드)
│   │   ├── crafting.c      # 장비 제작 (전리품 기반)
│   │   ├── encyclopedia.c  # 몬스터 도감 (연구 등급)
│   │   ├── event.c         # 등급별 랜덤 이벤트
│   │   ├── village.c       # 마을 활동 (알바/도박/대장간)
│   │   └── utils.c         # 공통 유틸리티 (UI 헬퍼, 바 그리기)
│   ├── include/            # 인터페이스 및 데이터 구조 정의 (.h)
│   │   ├── character.h     # Player 구조체, 어빌리티/도감 타입
│   │   ├── skill.h         # Skill 구조체, SkillType (모듈 분리)
│   │   ├── combat.h        # Monster, Dungeon 구조체
│   │   ├── job.h           # 16종 직업군 명세
│   │   ├── item.h          # 아이템/인벤토리 구조체
│   │   ├── generated_data.h# 자동 생성 몬스터/던전 데이터
│   │   └── ...             # 모듈별 헤더 파일
│   ├── scripts/            # 데이터 생성/검증 스크립트 (Python)
│   ├── data/               # 게임 데이터 JSON (monsters, dungeons)
│   └── build.bat           # GCC 빌드 자동화 스크립트
├── dashboard/              # 📊 Data Visualization
│   ├── src/                # React 소스 코드
│   │   ├── App.jsx         # 메인 대시보드 UI
│   │   └── data/           # 게임 데이터 연동 JSON
│   └── vite.config.js      # 빌드 설정
└── README.md               # 프로젝트 메인 문서
```

---

## ⚙️ 시작하기 (Getting Started)

### 사전 준비
- Windows OS 환경
- GCC 컴파일러 (MinGW 등) 설치
- Node.js (대시보드 실행 시)

### 빌드 및 실행 (TRPG)
1. 저장소 루트 폴더에서 아래 명령어를 실행합니다.
   ```bash
   cd trpg
   build.bat
   ```
2. 생성된 `trpg.exe`를 실행하세요! (이미 위 스크립트에서 자동 실행됩니다.)

### 대시보드 실행
1. `dashboard` 폴더로 이동합니다.
2. 아래 명령어를 순서대로 실행합니다.
   ```bash
   npm install
   npm run dev
   ```

---

## 🎮 게임 흐름 (Game Flow)

1. **캐릭터 생성**: 초기 능력치가 무작위로 부여됩니다.
2. **마을 활동**: 대장간에서 장비를 수리하거나, 훈련과 알바로 성장합니다.
3. **모험 & 사냥**: 5개의 던전(초심자의 들판 ~ 검은 용의 둥지)에서 적을 물리쳐 **전리품을 수집**하고 **도감을 채웁니다**.
4. **장비 제작**: 전리품을 모아 대장간에서 특수 장비를 제작합니다.
5. **전직 & 연구**: 레벨 10에 자동 전직이 진행되고, 도감 연구 단계가 오르면 전투 효율이 극대화됩니다.
6. **목표**: 최상위 전직인 '전설적인 아바타'에 도달하세요!

---

## 🔧 최근 리팩터링 이력 (v2.0 Refactoring)

전면적인 코드 리팩터링을 통해 다음 사항들이 개선되었습니다.

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
