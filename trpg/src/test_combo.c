#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/character.h"
#include "../include/combat.h"

int main() {
    Player p;
    memset(&p, 0, sizeof(Player));
    strcpy(p.name, "TestPlayer");
    p.job = JOB_WARRIOR;
    p.level = 20;
    p.skill_count = 0;

    init_skill_system();
    
    printf("[Test 1] Combo Skill Acquisition Check...\n");
    grant_combo_skill_if_eligible(&p);
    
    int has_skill = 0;
    for(int i=0; i<p.skill_count; i++) {
        if(strcmp(p.learned_skills[i].name, "[진·용참선]") == 0) {
            has_skill = 1;
            break;
        }
    }
    
    if(has_skill) printf("SUCCESS: Combo skill acquired!\n");
    else {
        printf("FAILED: Combo skill NOT acquired!\n");
        return 1;
    }

    printf("\n[Test 2] Combo Trigger Check (Dice: 4, 5, 6, 4, 5)...\n");
    int dice[5] = {4, 5, 6, 4, 5};
    int damage = calculate_yacht_damage(&p, dice);
    
    printf("Result Damage: %d\n", damage);
    if(damage == 19200) printf("SUCCESS: Combo triggered correctly!\n");
    else printf("FAILED: Combo trigger failed or wrong damage!\n");

    printf("\n[Test 3] No Combo Check (Dice: 1, 2, 3, 4, 5 - Large Straight)...\n");
    int dice2[5] = {1, 2, 3, 4, 5};
    int damage2 = calculate_yacht_damage(&p, dice2);
    printf("Result Damage: %d\n", damage2);
    if(damage2 == 4500) printf("SUCCESS: Standard rule preserved!\n");
    else printf("FAILED: Damage calculation mismatch!\n");

    return 0;
}
