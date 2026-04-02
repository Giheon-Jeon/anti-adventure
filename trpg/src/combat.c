#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/combat.h"
#include "../include/utils.h"

// 주사위 정렬 (오름차순)
void sort_dice(int* dice, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (dice[i] > dice[j]) {
                int temp = dice[i];
                dice[i] = dice[j];
                dice[j] = temp;
            }
        }
    }
}

// n개의 주사위 굴리기
void roll_dice(int* dice, int count) {
    for (int i = 0; i < count; i++) {
        dice[i] = (rand() % 6) + 1;
    }
}

// 야추 규칙 데미지 계산
int calculate_yacht_damage(Player* p, int dice[5]) {
    int counts[7] = {0}; // 1~6 눈금 개수
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        counts[dice[i]]++;
        sum += dice[i];
    }

    sort_dice(dice, 5);

    // 1. Yacht (5개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 5) {
            int mult = 30;
            const char* skill_name = "Yacht!";
            
            if (p->job == JOB_THIEF) { mult = 1000; skill_name = "데들리 럭!"; }
            else if (p->job == JOB_BERSERKER) { mult = 200; skill_name = "광란의 일격!"; }
            else if (p->job == JOB_ASSASSIN) { mult = 300; skill_name = "일격필살!"; }
            else if (p->job == JOB_JUDGE) { mult = 400; skill_name = "최후의 심판!"; }
            else if (p->job == JOB_AVATAR) { mult = 500; skill_name = "신성한 심판!"; }
            
            if (mult > 30) printf("[직업 스킬: %s (Yacht %d배)]\n", skill_name, mult);
            else printf("[조합: Yacht! (30배)]\n");
            
            return sum * 10 * mult;
        }
    }

    // 2. Large Straight (5개 연속)
    int is_large_straight = 1;
    for (int i = 0; i < 4; i++) {
        if (dice[i+1] != dice[i] + 1) {
            is_large_straight = 0;
            break;
        }
    }
    if (is_large_straight) {
        int mult = 25;
        const char* skill_name = "Large Straight!";
        
        if (p->job == JOB_MAGE) { mult = 150; skill_name = "메테오 스트라이크!"; }
        else if (p->job == JOB_RANGER) { mult = 80; skill_name = "화살 폭격!"; }
        else if (p->job == JOB_GRANDMAGE) { mult = 200; skill_name = "궁극의 지혜!"; }
        else if (p->job == JOB_CHAMPION) { mult = 100; skill_name = "영광의 길!"; }
        else if (p->job == JOB_AVATAR) { mult = 120; skill_name = "세상의 끝!"; }
        
        if (mult > 25) printf("[직업 스킬: %s (L.Straight %d배)]\n", skill_name, mult);
        else printf("[조합: Large Straight! (25배)]\n");
        return sum * 10 * mult;
    }

    // 3. Full House (3개 + 2개)
    int has_3 = 0, has_2 = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) has_3 = i;
        if (counts[i] == 2) has_2 = i;
    }
    if (has_3 && has_2) {
        int mult = 20;
        const char* skill_name = "Full House!";
        
        if (p->job == JOB_CRUSADER) { mult = 100; skill_name = "강화된 신성력!"; }
        else if (p->job == JOB_PALADIN) { mult = 150; skill_name = "성궤의 심판!"; }
        
        if (mult > 20) printf("[직업 스킬: %s (Full House %d배)]\n", skill_name, mult);
        else printf("[조합: Full House! (20배)]\n");
        return sum * 10 * mult;
    }

    // 4. Small Straight (4개 연속)
    int consecutive = 0, max_consecutive = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] > 0) consecutive++;
        else consecutive = 0;
        if (consecutive > max_consecutive) max_consecutive = consecutive;
    }
    if (max_consecutive >= 4) {
        int mult = 15;
        const char* skill_name = "Small Straight!";
        
        if (p->job == JOB_ARCHER) { mult = 45; skill_name = "샤프 아이즈!"; }
        else if (p->job == JOB_RANGER) { mult = 40; skill_name = "신속의 화살!"; }
        else if (p->job == JOB_CHAMPION) { mult = 60; skill_name = "용사의 연격!"; }
        else if (p->job == JOB_GRANDMAGE) { mult = 80; skill_name = "마력의 파동!"; }
        
        if (mult > 15) printf("[직업 스킬: %s (S.Straight %d배)]\n", skill_name, mult);
        else printf("[조합: Small Straight! (15배)]\n");
        return sum * 10 * mult;
    }

    // 5. Quad (4개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 4) {
            int mult = 10;
            const char* skill_name = "Quad!";
            
            if (p->job == JOB_BERSERKER) { mult = 50; skill_name = "무자비한 타격!"; }
            else if (p->job == JOB_ASSASSIN) { mult = 60; skill_name = "급소 찌르기!"; }
            else if (p->job == JOB_JUDGE) { mult = 80; skill_name = "철퇴 하강!"; }
            else if (p->job == JOB_PALADIN) { mult = 70; skill_name = "홀리 스트라이크!"; }
            
            if (mult > 10) printf("[직업 스킬: %s (Quad %d배)]\n", skill_name, mult);
            else printf("[조합: Quad! (10배)]\n");
            return (i * 4 * 10 * mult) + ((sum - i * 4) * 10);
        }
    }

    // 6. Triple (3개 동일)
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) {
            int mult = 5;
            const char* skill_name = "Triple!";
            
            if (p->job == JOB_WARRIOR) { mult = 12; skill_name = "파이널 어택!"; }
            else if (p->job == JOB_GLADIATOR) { mult = 15; skill_name = "쌍검 난무!"; }
            else if (p->job == JOB_SAGE) { mult = 20; skill_name = "지혜의 일격!"; }
            
            if (mult > 5) printf("[직업 스킬: %s (Triple %d배)]\n", skill_name, mult);
            else printf("[조합: Triple! (5배)]\n");
            return (i * 3 * 10 * mult) + ((sum - i * 3) * 10);
        }
    }

    // 7. Pair (2개 동일)
    for (int i = 6; i >= 1; i--) {
        if (counts[i] == 2) {
            int mult = 2;
            if (p->job == JOB_GLADIATOR || p->job == JOB_SAGE) {
                printf("[직업 스킬: 연쇄 반응! (Pair 6배)]\n");
                mult = 6;
            } else {
                printf("[조합: One Pair! (2배)]\n");
            }
            return (i * 2 * 10 * mult) + ((sum - i * 2) * 10);
        }
    }

    // 8. No Set (기본)
    printf("[조합: No Set (기본)]\n");
    return sum * 10;
}

int calculate_final_damage(Player* p, Monster* m, int yacht_result) {
    float base_stat_power = 0;
    switch (p->job) {
        case JOB_WARRIOR: base_stat_power = (p->str * 5.0f); break;
        case JOB_ARCHER:  base_stat_power = (p->dex * 5.0f); break;
        case JOB_MAGE:    base_stat_power = (p->intel * 5.0f); break;
        case JOB_THIEF:   base_stat_power = (p->luk * 5.0f); break;
        case JOB_GLADIATOR: base_stat_power = (p->str * 3.0f + p->dex * 3.0f); break;
        case JOB_CRUSADER:  base_stat_power = (p->str * 3.0f + p->intel * 3.0f); break;
        case JOB_BERSERKER: base_stat_power = (p->str * 3.0f + p->luk * 3.0f); break;
        case JOB_RANGER:    base_stat_power = (p->dex * 3.0f + p->intel * 3.0f); break;
        case JOB_ASSASSIN:  base_stat_power = (p->dex * 3.0f + p->luk * 3.0f); break;
        case JOB_SAGE:      base_stat_power = (p->intel * 3.0f + p->luk * 3.0f); break;
        case JOB_CHAMPION:  base_stat_power = (p->str * 2.5f + p->dex * 2.5f + p->intel * 2.5f); break;
        case JOB_JUDGE:     base_stat_power = (p->str * 2.5f + p->dex * 2.5f + p->luk * 2.5f); break;
        case JOB_PALADIN:   base_stat_power = (p->str * 2.5f + p->intel * 2.5f + p->luk * 2.5f); break;
        case JOB_GRANDMAGE: base_stat_power = (p->dex * 2.5f + p->intel * 2.5f + p->luk * 2.5f); break;
        case JOB_AVATAR:    base_stat_power = (p->str + p->dex + p->intel + p->luk) * 2.0f; break;
        default:          base_stat_power = (p->str * 4.0f + p->dex); break;
    }
    
    float skill_mult = yacht_result / 100.0f; 
    float damage = base_stat_power * skill_mult;
    damage *= (1.0f + p->dmg_percent);
    if (m->is_boss) damage *= (1.0f + p->boss_dmg);
    float effective_def = m->def * (1.0f - p->ied);
    if (effective_def < 0) effective_def = 0;
    int final_dmg = (int)(damage - effective_def);
    return (final_dmg < 1) ? 1 : final_dmg;
}

void start_combat(Player* p, Dungeon* d) {
    Monster enemy;
    int roll = rand() % 100;
    int enemy_rank = 0;

    // 몬스터 정보 결정 (던전에 따라 다름)
    if (d->has_boss && roll >= 80) { // 20%확률 보스
        enemy = d->boss;
    } else {
        int idx = rand() % d->monster_count;
        enemy = d->monsters[idx];
    }
    
    // 던전 수준에 따른 랭크 (보상용)
    if (d->min_cp < 500) enemy_rank = 0;
    else if (d->min_cp < 2000) enemy_rank = 1;
    else if (d->min_cp < 5000) enemy_rank = 2;
    else enemy_rank = 3;

    enemy.hp = enemy.max_hp;

    clear_screen();
    show_compact_status(p);
    printf("\n--- %s에 입장했습니다! ---\n", d->name);
    printf("--- %s(이)가 나타났다! (HP: %d, 방어율: %d) ---\n", enemy.name, enemy.hp, enemy.def);

    while (p->hp > 0 && enemy.hp > 0) {
        clear_screen();
        show_compact_status(p);
        printf("\n[%s HP: %d / %d]\n", enemy.name, enemy.hp, enemy.max_hp);
        
        int choice;
        printf("\n[플레이어 턴]\n1. 공격하기\n2. 도망가기\n선택: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 2) {
            int escape_dice[5];
            roll_dice(escape_dice, 5);
            sort_dice(escape_dice, 5); // 도망 주사위 정렬 추가
            int sum = 0;
            printf("> 도망 주사위 결과: [");
            for (int i = 0; i < 5; i++) {
                sum += escape_dice[i];
                printf("%d%s", escape_dice[i], (i == 4) ? "" : ", ");
            }
            printf("] (합계: %d)\n", sum);

            if (sum >= 15) {
                printf("✨ 무사히 도망쳤습니다! ✨\n");
                wait_for_enter();
                return;
            } else {
                printf("💀 도망에 실패했습니다! 공격 기회를 잃었습니다... 💀\n");
            }
        } else {
            int p_dice[5];
            roll_dice(p_dice, 5);
            sort_dice(p_dice, 5); // 주사위 정렬 추가
            printf("> 주사위 결과: [%d, %d, %d, %d, %d]\n", p_dice[0], p_dice[1], p_dice[2], p_dice[3], p_dice[4]);
            
            int yacht_score = calculate_yacht_damage(p, p_dice);
            int final_dmg = calculate_final_damage(p, &enemy, yacht_score);
            
            printf("> 데미지 계산 결과: [ %d ]\n", final_dmg);
            enemy.hp -= final_dmg;

            // 어빌리티: 연속 공격 체크
            float double_attack_prob = 0;
            for (int i = 0; i < ABILITY_COUNT; i++) {
                if (p->abilities[i].type == ABILITY_TYPE_DOUBLE_ATTACK) {
                    double_attack_prob += p->abilities[i].value;
                }
            }
            if (enemy.hp > 0 && (rand() % 100 < (int)double_attack_prob)) {
                printf("\n✨ [어빌리티] 연속 공격 발동! ✨\n");
                // 주사위를 새로 굴려 다시 공격 (또는 기존 데미지 재사용 가능하지만 새로 굴리는 것이 더 몰입감 있음)
                int p_dice2[5];
                roll_dice(p_dice2, 5);
                sort_dice(p_dice2, 5);
                printf("> 추가 주사위 결과: [%d, %d, %d, %d, %d]\n", p_dice2[0], p_dice2[1], p_dice2[2], p_dice2[3], p_dice2[4]);
                int yacht_score2 = calculate_yacht_damage(p, p_dice2);
                int final_dmg2 = calculate_final_damage(p, &enemy, yacht_score2);
                printf("> 추가 데미지 결과: [ %d ]\n", final_dmg2);
                enemy.hp -= final_dmg2;
                if (enemy.hp < 0) enemy.hp = 0;
            }
            if (enemy.hp < 0) enemy.hp = 0;

            if (enemy.hp <= 0) {
                int exp_gain = (enemy_rank + 1) * 100;
                int gold_gain = (enemy_rank + 1) * 200;
                if (enemy.is_boss) {
                    exp_gain *= 5; gold_gain *= 10;
                    printf("🏆 보물 같은 보스 처치! 🏆\n");
                }
                p->exp += exp_gain;
                p->gold += gold_gain;
                printf("EXP +%d, Gold +%d 획득!\n", exp_gain, gold_gain);
                
                if (p->inventory.count < MAX_INVENTORY_SIZE) {
                    p->inventory.items[p->inventory.count] = enemy.loot;
                    p->inventory.count++;
                    printf("🎁 전리품 획득: [%s] 🎁\n", enemy.loot.name);
                }
                check_level_up(p);
                wait_for_enter();
                return;
            }
        }

        // 적 턴
        printf("\n[%s의 턴] 공격합니다!\n", enemy.name);
        int e_dice[3];
        roll_dice(e_dice, 3);
        int e_dmg = (e_dice[0] + e_dice[1] + e_dice[2]) * (enemy_rank + 1) * 2;
        if (enemy.is_boss) e_dmg *= 3;
        printf("> %d의 데미지를 입었습니다!\n", e_dmg);
        p->hp -= e_dmg;
        if (p->hp < 0) p->hp = 0;

        if (p->hp <= 0) {
            apply_death_penalty(p);
            wait_for_enter();
            break;
        }
        wait_for_enter();
    }
}

void select_dungeon(Player* p) {
    Dungeon dungeons[5];
    
    // 1. 초심자의 들판
    strcpy(dungeons[0].name, "초심자의 들판");
    dungeons[0].min_cp = 0; dungeons[0].max_cp = 500;
    dungeons[0].monster_count = 2;
    strcpy(dungeons[0].monsters[0].name, "파란 달팽이");
    dungeons[0].monsters[0].max_hp = 300; dungeons[0].monsters[0].def = 5; dungeons[0].monsters[0].is_boss = 0;
    strcpy(dungeons[0].monsters[0].loot.name, "달팽이 껍질"); dungeons[0].monsters[0].loot.value = 10;
    strcpy(dungeons[0].monsters[1].name, "슬라임");
    dungeons[0].monsters[1].max_hp = 600; dungeons[0].monsters[1].def = 10; dungeons[0].monsters[1].is_boss = 0;
    strcpy(dungeons[0].monsters[1].loot.name, "슬라임의 핵"); dungeons[0].monsters[1].loot.value = 20;
    dungeons[0].has_boss = 0;

    // 2. 노란 버섯 동산
    strcpy(dungeons[1].name, "노란 버섯 동산");
    dungeons[1].min_cp = 500; dungeons[1].max_cp = 2000;
    dungeons[1].monster_count = 1;
    strcpy(dungeons[1].monsters[0].name, "주황버섯");
    dungeons[1].monsters[0].max_hp = 1500; dungeons[1].monsters[0].def = 30; dungeons[1].monsters[0].is_boss = 0;
    strcpy(dungeons[1].monsters[0].loot.name, "버섯 갓"); dungeons[1].monsters[0].loot.value = 50;
    dungeons[1].has_boss = 1;
    strcpy(dungeons[1].boss.name, "🍄 머쉬맘 (BOSS) 🍄");
    dungeons[1].boss.max_hp = 10000; dungeons[1].boss.def = 100; dungeons[1].boss.is_boss = 1;
    strcpy(dungeons[1].boss.loot.name, "머쉬맘의 포자"); dungeons[1].boss.loot.value = 500;

    // 3. 축축한 습지
    strcpy(dungeons[2].name, "축축한 습지");
    dungeons[2].min_cp = 2000; dungeons[2].max_cp = 5000;
    dungeons[2].monster_count = 2;
    strcpy(dungeons[2].monsters[0].name, "오크");
    dungeons[2].monsters[0].max_hp = 8000; dungeons[2].monsters[0].def = 80; dungeons[2].monsters[0].is_boss = 0;
    strcpy(dungeons[2].monsters[0].loot.name, "오크의 이빨"); dungeons[2].monsters[0].loot.value = 150;
    strcpy(dungeons[2].monsters[1].name, "와일드카고");
    dungeons[2].monsters[1].max_hp = 15000; dungeons[2].monsters[1].def = 150; dungeons[2].monsters[1].is_boss = 0;
    strcpy(dungeons[2].monsters[1].loot.name, "와일드카고의 눈동자"); dungeons[2].monsters[1].loot.value = 250;
    dungeons[2].has_boss = 1;
    strcpy(dungeons[2].boss.name, "🔥 자쿰의 팔 (BOSS) 🔥");
    dungeons[2].boss.max_hp = 50000; dungeons[2].boss.def = 300; dungeons[2].boss.is_boss = 1;
    strcpy(dungeons[2].boss.loot.name, "자쿰의 나뭇가지"); dungeons[2].boss.loot.value = 2000;

    // 4. 잊혀진 신전
    strcpy(dungeons[3].name, "잊혀진 신전");
    dungeons[3].min_cp = 5000; dungeons[3].max_cp = 15000;
    dungeons[3].monster_count = 2;
    strcpy(dungeons[3].monsters[0].name, "스켈레톤");
    dungeons[3].monsters[0].max_hp = 50000; dungeons[3].monsters[0].def = 300; dungeons[3].monsters[0].is_boss = 0;
    strcpy(dungeons[3].monsters[0].loot.name, "부서진 뼈"); dungeons[3].monsters[0].loot.value = 600;
    strcpy(dungeons[3].monsters[1].name, "다크 레인저");
    dungeons[3].monsters[1].max_hp = 80000; dungeons[3].monsters[1].def = 400; dungeons[3].monsters[1].is_boss = 0;
    strcpy(dungeons[3].monsters[1].loot.name, "검은 화살촉"); dungeons[3].monsters[1].loot.value = 1000;
    dungeons[3].has_boss = 1;
    strcpy(dungeons[3].boss.name, "🕒 파풀라투스 (BOSS) 🕒");
    dungeons[3].boss.max_hp = 300000; dungeons[3].boss.def = 600; dungeons[3].boss.is_boss = 1;
    strcpy(dungeons[3].boss.loot.name, "시계 파편"); dungeons[3].boss.loot.value = 8000;

    // 5. 검은 용의 둥지
    strcpy(dungeons[4].name, "검은 용의 둥지");
    dungeons[4].min_cp = 15000; dungeons[4].max_cp = 999999;
    dungeons[4].monster_count = 2;
    strcpy(dungeons[4].monsters[0].name, "블루 와이번");
    dungeons[4].monsters[0].max_hp = 200000; dungeons[4].monsters[0].def = 600; dungeons[4].monsters[0].is_boss = 0;
    strcpy(dungeons[4].monsters[0].loot.name, "와이번의 발톱"); dungeons[4].monsters[0].loot.value = 2500;
    strcpy(dungeons[4].monsters[1].name, "다크 코르니안");
    dungeons[4].monsters[1].max_hp = 400000; dungeons[4].monsters[1].def = 800; dungeons[4].monsters[1].is_boss = 0;
    strcpy(dungeons[4].monsters[1].loot.name, "단단한 비늘"); dungeons[4].monsters[1].loot.value = 4000;
    dungeons[4].has_boss = 1;
    strcpy(dungeons[4].boss.name, "🌸 핑크빈 (BOSS) 🌸");
    dungeons[4].boss.max_hp = 1200000; dungeons[4].boss.def = 1200; dungeons[4].boss.is_boss = 1;
    strcpy(dungeons[4].boss.loot.name, "핑크빈의 인형"); dungeons[4].boss.loot.value = 30000;

    while(1) {
        clear_screen();
        show_compact_status(p);
        printf("\n========= [사냥터 선택] =========\n");
        printf("현재 전투력에 맞는 던전을 선택하세요.\n\n");
        
        for(int i=0; i<5; i++) {
            printf("%d. %s (권장 CP: %d ~ ", i+1, dungeons[i].name, dungeons[i].min_cp);
            if(dungeons[i].max_cp > 100000) printf("MAX)\n");
            else printf("%d)\n", dungeons[i].max_cp);
            printf("   - 등장: %s, %s", dungeons[i].monsters[0].name, dungeons[i].monsters[1].name);
            if(dungeons[i].has_boss) printf(", %s", dungeons[i].boss.name);
            printf("\n");
        }
        printf("0. 뒤로 가기\n");
        printf("선택: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        if (choice == 0) break;
        if (choice < 1 || choice > 5) {
            printf("[알림] 잘못된 선택입니다.\n");
            wait_for_enter();
            continue;
        }
        
        Dungeon *selected = &dungeons[choice-1];
        if (p->combat_power < selected->min_cp) {
            printf("\n⚠️ [경고] 전투력이 권장 수치보다 낮습니다! 진입하시겠습니까?\n");
            printf("1. 예 (위험 감수)\n2. 아니오 (마을로 돌아감)\n선택: ");
            int confirm;
            scanf("%d", &confirm);
            clear_input_buffer();
            if (confirm != 1) continue;
        }
        
        // 던전 진입 (단순하게 전투 바로 시작 또는 이벤트 발생)
        // 여기서는 기존 이벤트 시스템과 통합하거나 바로 start_combat 호출
        // 사용자가 "전투하러 가면... 알려줘"라고 했으므로 바로 전투 또는 던전 전용 이벤트
        start_combat(p, selected);
        break;
    }
}
