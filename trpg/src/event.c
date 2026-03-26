#include <stdio.h>
#include <stdlib.h>
#include "../include/event.h"
#include "../include/combat.h"

void trigger_event(Player* p) {
    int event_chance = rand() % 100;

    if (event_chance < 60) {
        // 60% 확률로 전투 발생
        start_combat(p);
    } else {
        // 40% 확률로 일반 텍스트 이벤트 발생
        printf("\n--- 무작위 이벤트 발생! ---\n");
        int sub_event = rand() % 3;
        
        switch (sub_event) {
            case 0:
                printf("길가에서 누군가 잃어버린 주머니를 발견했습니다!\n");
                printf("Gold +30 획득!\n");
                p->gold += 30;
                break;
            case 1:
                printf("샘터에서 맑은 물을 마셔 기운이 납니다.\n");
                printf("HP가 조금 회복되었습니다.\n");
                p->hp += 20;
                if (p->hp > p->max_hp) p->hp = p->max_hp;
                break;
            case 2:
                printf("오래된 비석을 조사하다가 지혜를 얻었습니다.\n");
                printf("EXP +10 획득!\n");
                p->exp += 10;
                break;
        }
    }
}
