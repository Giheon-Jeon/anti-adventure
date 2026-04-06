#include <stdio.h>
#include <stdlib.h>
#include "../include/character.h"
#include "../include/ability.h"

void init_player(Player* p) {
    printf("--- 캐릭터 생성 ---\n");
    printf("이름을 입력하세요: ");
    scanf("%s", p->name);
    
    p->job = JOB_NONE;
    p->level = 1;
    p->hp = 100;
    p->max_hp = 100;
    p->mp = 50;
    p->max_mp = 50;
    
    p->str = 10;
    p->dex = 10;
    p->intel = 10;
    p->luk = 10;
    
    p->magic_atk = 0;
    p->ied = 0.0f;
    p->boss_dmg = 0.0f;
    p->dmg_percent = 0.0f;
    
    p->exp = 0;
    p->gold = 100;
    p->stat_points = 0;
    p->weapon_tier = 0;
    p->armor_tier = 0;
    p->accessory_tier = 0;
    p->c_weapon_tier = 0;
    p->c_armor_tier = 0;
    p->c_accessory_tier = 0;
    
    for (int i = 0; i < ABILITY_COUNT; i++) {
        p->abilities[i].type = ABILITY_TYPE_NONE;
        p->abilities[i].rank = ABILITY_RANK_NORMAL;
        p->abilities[i].value = 0;
        p->ability_locked[i] = 0;
    }
    p->last_job_time = 0;
    p->job_count = 0;
    
    p->inventory.count = 0;
    
    p->skill_count = 0;
    p->has_ultimate = 0;
    init_skill_system();
    
    update_combat_power(p);
    printf("\n'%s'님, 모험을 시작합니다!\n", p->name);
}

// distribute_stats 함수는 더 이상 사용되지 않음 (사용자 규칙: 자동/랜덤 성장)

void check_level_up(Player* p) {
    int required_exp = p->level * 100;
    if (p->exp >= required_exp) {
        p->level++;
        p->exp -= required_exp;
        
        // 기본 스탯 상승 (HP/MP)
        p->max_hp += 20;
        p->hp = p->max_hp;
        p->max_mp += 10;
        p->mp = p->max_mp;
        
        p->stat_points += 5;
        
        printf("\n🎉 레벨 업! [Level %d]가 되었습니다! 🎉\n", p->level);

        // 1. 전직 체크
        if (p->job == JOB_NONE && p->level >= JOB_ADVANCEMENT_LEVEL) {
            printf("\n✨ 운명에 따라 당신의 길이 결정됩니다... ✨\n");
            
            int s = p->str, d = p->dex, i = p->intel, l = p->luk;
            int max_v = s;
            if (d > max_v) max_v = d;
            if (i > max_v) max_v = i;
            if (l > max_v) max_v = l;

            int is_s = (s == max_v), is_d = (d == max_v), is_i = (i == max_v), is_l = (l == max_v);
            int count = is_s + is_d + is_i + is_l;

            if (count == 4) p->job = JOB_AVATAR;
            else if (count == 3) {
                if (!is_l) p->job = JOB_CHAMPION;
                else if (!is_i) p->job = JOB_JUDGE;
                else if (!is_d) p->job = JOB_PALADIN;
                else p->job = JOB_GRANDMAGE;
            } else if (count == 2) {
                if (is_s && is_d) p->job = JOB_GLADIATOR;
                else if (is_s && is_i) p->job = JOB_CRUSADER;
                else if (is_s && is_l) p->job = JOB_BERSERKER;
                else if (is_d && is_i) p->job = JOB_RANGER;
                else if (is_d && is_l) p->job = JOB_ASSASSIN;
                else p->job = JOB_SAGE;
            } else {
                if (is_s) p->job = JOB_WARRIOR;
                else if (is_d) p->job = JOB_ARCHER;
                else if (is_i) p->job = JOB_MAGE;
                else p->job = JOB_THIEF;
            }

            const char* j_names[] = {
                "초보자", "전사", "궁수", "마법사", "도적",
                "검투사", "성기사", "광전사", "레인저", "암살자", "현자",
                "초인", "심판자", "성궤사", "대마법사", "아바타"
            };
            printf("가장 높은 능력치에 따른 조율 결과, [ %s ](으)로 결정되었습니다!\n", j_names[p->job]);
        }

        // 2. 능력치 무작위 분배
        int pts = p->stat_points;
        p->stat_points = 0;
        
        if (p->job == JOB_NONE) {
            printf("능력치 5포인트가 무작위로 분배됩니다.\n");
            for(int k=0; k<pts; k++) {
                int r = rand() % 4;
                if(r==0) { p->str++; printf("STR +1 "); }
                else if(r==1) { p->dex++; printf("DEX +1 "); }
                else if(r==2) { p->intel++; printf("INT +1 "); }
                else { p->luk++; printf("LUK +1 "); }
            }
            printf("\n");
        } else {
            // 전직 후: 해당 직업의 주력 스탯들 사이에서 무작위 분배
            int targets[4] = {0};
            int t_count = 0;
            
            switch(p->job) {
                case JOB_WARRIOR: targets[0]=1; t_count=1; break;
                case JOB_ARCHER:  targets[1]=1; t_count=1; break;
                case JOB_MAGE:    targets[2]=1; t_count=1; break;
                case JOB_THIEF:   targets[3]=1; t_count=1; break;
                case JOB_GLADIATOR: targets[0]=targets[1]=1; t_count=2; break;
                case JOB_CRUSADER:  targets[0]=targets[2]=1; t_count=2; break;
                case JOB_BERSERKER: targets[0]=targets[3]=1; t_count=2; break;
                case JOB_RANGER:    targets[1]=targets[2]=1; t_count=2; break;
                case JOB_ASSASSIN:  targets[1]=targets[3]=1; t_count=2; break;
                case JOB_SAGE:      targets[2]=targets[3]=1; t_count=2; break;
                case JOB_CHAMPION:  targets[0]=targets[1]=targets[2]=1; t_count=3; break;
                case JOB_JUDGE:     targets[0]=targets[1]=targets[3]=1; t_count=3; break;
                case JOB_PALADIN:   targets[0]=targets[2]=targets[3]=1; t_count=3; break;
                case JOB_GRANDMAGE: targets[1]=targets[2]=targets[3]=1; t_count=3; break;
                case JOB_AVATAR:    targets[0]=targets[1]=targets[2]=targets[3]=1; t_count=4; break;
                default: break;
            }
            
            printf("직업 특성에 따라 주력 능력치가 상승합니다.\n");
            for(int k=0; k<pts; k++) {
                int r_idx = rand() % t_count;
                int current = 0;
                for(int j=0; j<4; j++) {
                    if(targets[j]) {
                        if(current == r_idx) {
                            if(j==0) { p->str++; printf("STR +1 "); }
                            else if(j==1) { p->dex++; printf("DEX +1 "); }
                            else if(j==2) { p->intel++; printf("INT +1 "); }
                            else { p->luk++; printf("LUK +1 "); }
                            break;
                        }
                        current++;
                    }
                }
            }
            printf("\n");
        }
        
        // 3. 스킬 선택
        select_level_up_skill(p);

        update_combat_power(p);
        check_level_up(p); 
    }
}

const char* get_job_name(JobType job) {
    static const char* names[] = {
        "초보자", "전사", "궁수", "마법사", "도적",
        "검투사", "성기사", "광전사", "레인저", "암살자", "현자",
        "초인", "심판자", "성궤사", "대마법사", "아바타"
    };
    return names[job];
}

void show_status(Player* p) {
    update_combat_power(p);
    printf("\n====== [%s] 상태 (Level %d / %s) ======\n", p->name, p->level, get_job_name(p->job));
    printf("HP: %d / %d | MP: %d / %d\n", p->hp, p->max_hp, p->mp, p->max_mp);
    printf("--------------------------\n");
    printf("STR: %d | DEX: %d\n", p->str, p->dex);
    printf("INT: %d | LUK: %d\n", p->intel, p->luk);
    printf("--------------------------\n");
    printf("마력: %d | 방무: %.1f%%\n", p->magic_atk, p->ied * 100.0f);
    printf("보뎀: %.1f%% | 뎀퍼: %.1f%%\n", p->boss_dmg * 100.0f, p->dmg_percent * 100.0f);
    printf("--------------------------\n");
    printf("EXP: %d / %d | Gold: %d\n", p->exp, p->level * 100, p->gold);
    printf("--------------------------\n");
    printf("장비 등급 (상점): [무기 T%d] [방어구 T%d] [장신구 T%d]\n", p->weapon_tier, p->armor_tier, p->accessory_tier);
    printf("장비 등급 (연구): [무기 T%d] [방어구 T%d] [장신구 T%d]\n", p->c_weapon_tier, p->c_armor_tier, p->c_accessory_tier);
    
    printf("--------------------------\n");
    printf("[어빌리티 능력]\n");
    int has_ability = 0;
    for (int i = 0; i < ABILITY_COUNT; i++) {
        if (p->abilities[i].type != ABILITY_TYPE_NONE) {
            printf("- %d번: [%s] 수치: %.0f%s\n", 
                   i + 1, get_ability_rank_name(p->abilities[i].rank), p->abilities[i].value,
                   (p->abilities[i].type >= ABILITY_TYPE_STR_PER ? "%" : ""));
            has_ability = 1;
        }
    }
    if (!has_ability) printf("- 개방된 능력이 없습니다.\n");

    printf("--------------------------\n");
    show_skills(p);

    printf("전투력: %d\n", p->combat_power);
    printf("========================\n");
}

void show_compact_status(Player* p) {
    update_combat_power(p);
    printf("\n[ %s | %s LV %d | HP: %d/%d | CP: %d | Gold: %d G ]\n", 
           p->name, get_job_name(p->job), p->level, p->hp, p->max_hp, p->combat_power, p->gold);
    printf("[ 기본T:%d/%d/%d | 제작T:%d/%d/%d ]\n", 
           p->weapon_tier, p->armor_tier, p->accessory_tier, p->c_weapon_tier, p->c_armor_tier, p->c_accessory_tier);
    printf("[ STR:%d DEX:%d INT:%d LUK:%d ]", p->str, p->dex, p->intel, p->luk);
    
    // 추가: 특수 능력치가 있을 경우만 표시
    if (p->ied > 0 || p->boss_dmg > 0 || p->dmg_percent > 0) {
        printf(" [ ");
        if (p->ied > 0) printf("IED:%.0f%% ", p->ied * 100);
        if (p->boss_dmg > 0) printf("Boss:%.0f%% ", p->boss_dmg * 100);
        if (p->dmg_percent > 0) printf("Dmg:%.0f%% ", p->dmg_percent * 100);
        printf("]");
    }
    printf("\n------------------------------------------------------------\n");
}

void show_inventory(const Player* p) {
    printf("\n========= [인벤토리] (%d / %d) =========\n", p->inventory.count, MAX_INVENTORY_SIZE);
    if (p->inventory.count == 0) {
        printf("비어 있습니다.\n");
    } else {
        for (int i = 0; i < p->inventory.count; i++) {
            printf("%d. [%s] - %s (가치: %d G)\n", 
                   i + 1, p->inventory.items[i].name, p->inventory.items[i].description, p->inventory.items[i].value);
        }
    }
    printf("========================================\n");
    printf("엔터를 누르면 돌아갑니다...");
    while (getchar() != '\n'); // 이전 입력 버퍼 비우기용 (안정성)
    getchar(); 
}
void apply_death_penalty(Player* p) {
    printf("\n💀 [사망] 전투에서 패배했습니다! 💀\n");
    printf("강력한 사망 패널티가 적용됩니다.\n");

    // 1. 골드 감소 (50%)
    int gold_loss = p->gold / 2;
    p->gold -= gold_loss;
    printf("- 소지 골드 절반 소실: -%d G\n", gold_loss);

    // 2. 경험치 초기화 (현재 레벨의 0%)
    p->exp = 0;
    printf("- 현재 레벨 경험치 초기화: 0%%\n");

    // 3. 장비 등급 하락 (각 20% 확률)
    if (p->weapon_tier > 0 && (rand() % 100 < 20)) {
        p->weapon_tier--;
        printf("- [경고] 무기가 손상되어 등급이 하락했습니다! (Tier %d)\n", p->weapon_tier);
        // 실제 스탯 차감을 위해 장비 효과는 상점에서 다시 사거나...
        // 여기서는 그냥 티어만 낮춤 (상점 로직상 티어가 낮으면 스탯이 낮음)
    }
    if (p->armor_tier > 0 && (rand() % 100 < 20)) {
        p->armor_tier--;
        printf("- [경고] 방어구가 파손되어 등급이 하락했습니다! (Tier %d)\n", p->armor_tier);
    }
    if (p->accessory_tier > 0 && (rand() % 100 < 20)) {
        p->accessory_tier--;
        printf("- [경고] 장신구가 빛을 잃어 등급이 하락했습니다! (Shop Tier %d)\n", p->accessory_tier);
    }
    
    // 제작 아이템 하락 (15% 확률)
    if (p->c_weapon_tier > 0 && (rand() % 100 < 15)) {
        p->c_weapon_tier--;
        printf("- [손실] 제작 무기의 마력이 흩어져 등급이 하락했습니다! (Craft Tier %d)\n", p->c_weapon_tier);
    }
    if (p->c_armor_tier > 0 && (rand() % 100 < 15)) {
        p->c_armor_tier--;
        printf("- [손실] 제작 방어구가 파손되어 등급이 하락했습니다! (Craft Tier %d)\n", p->c_armor_tier);
    }
    if (p->c_accessory_tier > 0 && (rand() % 100 < 15)) {
        p->c_accessory_tier--;
        printf("- [손실] 제작 장신구의 힘이 약해져 등급이 하락했습니다! (Craft Tier %d)\n", p->c_accessory_tier);
    }

    // 4. HP/MP 복구 (최소한의 생존)
    p->hp = p->max_hp / 10;
    if (p->hp < 1) p->hp = 1;
    p->mp = p->max_mp / 10;

    printf("\n겨우 목숨만 건져 마을로 돌아왔습니다...\n");
    update_combat_power(p);
}

void update_combat_power(Player* p) {
    float str_bonus = 0, dex_bonus = 0, int_bonus = 0, luk_bonus = 0;
    float str_per = 1.0f, dex_per = 1.0f, int_per = 1.0f, luk_per = 1.0f;
    float extra_ied = 0, extra_boss = 0, extra_dmg = 0;

    // 어빌리티 효과 집계
    for (int i = 0; i < ABILITY_COUNT; i++) {
        Ability a = p->abilities[i];
        switch (a.type) {
            case ABILITY_TYPE_STR_FLAT: str_bonus += a.value; break;
            case ABILITY_TYPE_DEX_FLAT: dex_bonus += a.value; break;
            case ABILITY_TYPE_INT_FLAT: int_bonus += a.value; break;
            case ABILITY_TYPE_LUK_FLAT: luk_bonus += a.value; break;
            case ABILITY_TYPE_STR_PER: str_per += a.value / 100.0f; break;
            case ABILITY_TYPE_DEX_PER: dex_per += a.value / 100.0f; break;
            case ABILITY_TYPE_INT_PER: int_per += a.value / 100.0f; break;
            case ABILITY_TYPE_LUK_PER: luk_per += a.value / 100.0f; break;
            case ABILITY_TYPE_BOSS_DMG: extra_boss += a.value / 100.0f; break;
            case ABILITY_TYPE_IED: extra_ied += a.value / 100.0f; break;
            case ABILITY_TYPE_DMG_PER: extra_dmg += a.value / 100.0f; break;
            default: break;
        }
    }

    // 최종 스택 계산 (CP용)
    float f_str = (p->str + str_bonus) * str_per;
    float f_dex = (p->dex + dex_bonus) * dex_per;
    float f_int = (p->intel + int_bonus) * int_per;
    float f_luk = (p->luk + luk_bonus) * luk_per;
    float f_ied = p->ied + extra_ied;
    float f_boss = p->boss_dmg + extra_boss;
    float f_dmg = p->dmg_percent + extra_dmg;

    float stat_sum = f_str + f_dex + f_int + f_luk;
    float cp = (stat_sum * 10.0f) + (p->max_hp / 10.0f) + (p->max_mp / 10.0f) + (p->magic_atk * 5.0f);
    cp += (f_ied * 2000.0f) + (f_boss * 2000.0f) + (f_dmg * 2000.0f);
    
    // 스킬에 의한 전투력 보너스 (레벨당 가산)
    for (int i = 0; i < p->skill_count; i++) {
        cp += p->learned_skills[i].level * 200;
        if (p->learned_skills[i].base_atk_bonus > 0) cp += p->learned_skills[i].base_atk_bonus * 5;
    }
    if (p->has_ultimate) cp += 5000;

    p->combat_power = (int)cp;
}
