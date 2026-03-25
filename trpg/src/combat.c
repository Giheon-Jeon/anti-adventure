#include <stdio.h>
#include "../include/combat.h"

// TODO: 학생 스스로 전투 로직을 구현해보세요!
// 힌트: 난수를 활용해 적의 공격력을 정하고, 플레이어와 적이 번갈아 공격하는 while 루프를 만드세요.
void start_combat(Player* p) {
    printf("\n--- 전투가 발생했습니다! ---\n");
    printf("[알림] 아직 전투 로직이 구현되지 않았습니다. combat.c 파일을 수정하여 완성해보세요!\n");
    
    // 단순 예시 (적 처치 시 경험치와 골드 획득)
    p->exp += 20;
    p->gold += 50;
    printf("임시 결과: EXP +20, Gold +50\n");
}
