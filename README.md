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

### 4. 데이터 대시보드 (Visual Studio)
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
│   │   ├── character.c     # 스탯 성장 및 전직 시스템
│   │   ├── combat.c        # 전투 엔진 및 턴 처리
│   │   ├── encyclopedia.c  # 몬스터 도감/연구 로직 (New)
│   │   ├── village.c       # 마을/대장간 수리 시스템 (New)
│   │   └── utils.c         # 공통 유틸리티 (로딩, 컬러링 등)
│   ├── include/            # 인터페이스 및 데이터 구조 정의 (.h)
│   │   ├── job.h           # 16종 직업군 명세
│   │   ├── generated_data.h# 생성된 몬스터/던전 데이터
│   │   └── ...             # 모듈별 헤더 파일
│   └── build/              # 컴파일된 .exe 파일 저장 경로
├── dashboard/              # 📊 Data Visualization (New)
│   ├── src/                # React 소스 코드
│   │   ├── App.jsx         # 메인 대시보드 UI
│   │   └── data/           # 게임 데이터 연동 JSON
│   └── vite.config.js      # 빌드 설정
├── build.bat               # 프로젝트 통합 빌드 스크립트
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
2. **마을 활동**: 대장간에서 장비를 수리하거나 훈련으로 성장합니다.
3. **모험 & 사냥**: 던전에서 적을 물리쳐 도감을 채우고 보상을 얻습니다.
4. **전직 & 연구**: 도감 연구 단계가 오르면 전투 효율이 극대화됩니다.
5. **목표**: 최상위 전직인 '전설적인 아바타'에 도달하세요!
