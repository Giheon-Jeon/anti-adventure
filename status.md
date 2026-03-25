### [2026-03-25 / TRPG 프로젝트 초기 설정 세션 업데이트]
- **학습 내용 (Topics Covered):**
  - C 언어 멀티파일 프로젝트의 구조 이해 (src, include 폴더 분리)
  - 헤더 파일(`.h`)과 소스 파일(`.c`) 간의 포함(include) 관계 및 상대 경로 학습
  - GCC를 이용한 다중 소스 파일 컴파일 및 링크 기초
  - Windows 배치 파일(`build.bat`)을 통한 빌드 자동화 맛보기
- **주요 성취 및 강점 (Strengths & Progress):**
  - 깔끔한 폴더 구조를 갖춘 프로젝트의 시작을 성공적으로 지원함.
- **취약점 및 고질적 실수 (Vulnerabilities & Chronic Mistakes):**
  - 멀티파일 프로젝트에서 단일 파일만 컴파일하려고 시도하는 경향 (링크 에러 발생 위험).
  - 헤더 파일 경로 설정에 대한 혼동.
- **다음 학습을 위한 가이드 (Actionable Insights & Review Plan):**
  - `combat.c`의 `while` 루프를 통한 로직 구현 실습 진행 예정.
  - 구조체 포인터(`Player* p`)를 함수 인자로 전달하는 방식의 중요성을 다시 한번 강조할 것.

### [2026-03-25 / w2 세션 업데이트]
- **학습 내용 (Topics Covered):**
  - 문자 인코딩(Character Encoding)의 개념: UTF-8 vs CP949
  - Windows 터미널에서 `system("chcp 65001")`을 통한 한글 깨짐 현상 해결
  - `stdlib.h` 라이브러리의 `system()` 함수 활용법
- **주요 성취 및 강점 (Strengths & Progress):**
  - 컴파일 성공 후 발생한 인코딩 문제를 원인 분석부터 해결 코드 적용까지 스스로(에이전트 가이드 포함) 이해함.
  - 소스 코드와 실행 환경 간의 차이를 이해하기 시작함.
- **취약점 및 고질적 실수 (Vulnerabilities & Chronic Mistakes):**
  - 소스 코드 저장 인코딩 설정을 간과하여 발생하는 출력 오류에 대한 경험 부족 (이번 세션으로 보완).
- **다음 학습을 위한 가이드 (Actionable Insights & Review Plan):**
  - `combat.c`의 로직 구현 시에도 한글 출력에 문제가 없는지 지속적으로 확인할 것.
  - `scanf`로 한글 이름을 입력받았을 때의 처리 방식에 대해서도 추후 학습 권장.
