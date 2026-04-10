# GCC 공식 이미지를 사용합니다.
FROM gcc:latest

# 데이터 생성을 위해 Python을 설치합니다.
RUN apt-get update && apt-get install -y python3

# 작업 디렉토리 설정
WORKDIR /app

# 현재 디렉토리의 파일을 컨테이너로 복사합니다.
COPY . .

# Python 실행 파일 이름을 통일합니다 (py -> python3)
# Makefile의 'py' 명령어를 컨테이너 환경에 맞게 조정하거나
# 여기서 직접 스크립트를 실행하여 헤더를 생성합니다.
RUN python3 scripts/generate_data.py

# 게임을 빌드합니다.
RUN make

# 터미널 인터랙티브 모드를 위해 게임을 실행할 명령어를 설정합니다.
# 실제 실행은 docker-compose나 docker run -it를 통해 수행됩니다.
CMD ["./trpg_game"]
