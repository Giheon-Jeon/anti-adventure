#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <direct.h>   // _mkdir (Windows)
    #include <io.h>        // _findfirst, _findnext (Windows)
#else
    #include <sys/stat.h>  // mkdir (POSIX)
    #include <sys/types.h>
    #include <dirent.h>    // opendir, readdir (POSIX)
#endif

#include "../include/saveload.h"
#include "../include/utils.h"

// --- saves 디렉터리 존재 확인 및 생성 ---
static void ensure_save_dir() {
#ifdef _WIN32
    _mkdir(SAVE_DIR);
#else
    mkdir(SAVE_DIR, 0755);
#endif
}

// ============================================================
//  세이브 (Player → CSV 섹션 파일)
// ============================================================
int save_game(Player* p, const char* filepath) {
    ensure_save_dir();

    FILE* fp = fopen(filepath, "w");
    if (!fp) {
        printf(RED "  [오류] 파일을 열 수 없습니다: %s\n" RESET, filepath);
        return 0;
    }

    // --- [PLAYER] 섹션 ---
    fprintf(fp, "[PLAYER]\n");
    fprintf(fp, "name,job,level,hp,max_hp,mp,max_mp,str,dex,intel,luk,"
                "magic_atk,ied,boss_dmg,dmg_percent,"
                "exp,gold,stat_points,skill_points,"
                "weapon_tier,armor_tier,accessory_tier,"
                "c_weapon_tier,c_armor_tier,c_accessory_tier,"
                "weapon_dur,armor_dur,accessory_dur,"
                "c_weapon_dur,c_armor_dur,c_accessory_dur,"
                "combat_power,last_job_time,job_count\n");
    fprintf(fp, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
                "%d,%.4f,%.4f,%.4f,"
                "%d,%d,%d,%d,"
                "%d,%d,%d,"
                "%d,%d,%d,"
                "%d,%d,%d,"
                "%d,%d,%d,"
                "%d,%lld,%d\n",
            p->name, (int)p->job, p->level, p->hp, p->max_hp, p->mp, p->max_mp,
            p->str, p->dex, p->intel, p->luk,
            p->magic_atk, p->ied, p->boss_dmg, p->dmg_percent,
            p->exp, p->gold, p->stat_points, p->skill_points,
            p->weapon_tier, p->armor_tier, p->accessory_tier,
            p->c_weapon_tier, p->c_armor_tier, p->c_accessory_tier,
            p->weapon_dur, p->armor_dur, p->accessory_dur,
            p->c_weapon_dur, p->c_armor_dur, p->c_accessory_dur,
            p->combat_power, (long long)p->last_job_time, p->job_count);

    // --- [ABILITIES] 섹션 ---
    fprintf(fp, "\n[ABILITIES]\n");
    fprintf(fp, "rank,type,value,locked\n");
    for (int i = 0; i < ABILITY_COUNT; i++) {
        fprintf(fp, "%d,%d,%.2f,%d\n",
                (int)p->abilities[i].rank, (int)p->abilities[i].type,
                p->abilities[i].value, p->ability_locked[i]);
    }

    // --- [INVENTORY] 섹션 ---
    fprintf(fp, "\n[INVENTORY]\n");
    fprintf(fp, "count=%d\n", p->inventory.count);
    fprintf(fp, "name,description,type,value\n");
    for (int i = 0; i < p->inventory.count; i++) {
        // 이름/설명에 쉼표가 들어갈 수 있으므로 따옴표로 감싸기
        fprintf(fp, "\"%s\",\"%s\",%d,%d\n",
                p->inventory.items[i].name,
                p->inventory.items[i].description,
                (int)p->inventory.items[i].type,
                p->inventory.items[i].value);
    }

    // --- [SKILLS] 섹션 ---
    fprintf(fp, "\n[SKILLS]\n");
    fprintf(fp, "count=%d\n", p->skill_count);
    fprintf(fp, "id,name,description,level,max_level,multiplier,base_atk_bonus,"
                "luk_bonus,hp_bonus,mp_bonus,str_bonus,dex_bonus,int_bonus,"
                "mp_cost,type,required_job,tier,prereq_id,is_passive,is_active\n");
    for (int i = 0; i < p->skill_count; i++) {
        Skill* s = &p->learned_skills[i];
        fprintf(fp, "%d,\"%s\",\"%s\",%d,%d,%.4f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                s->id, s->name, s->description,
                s->level, s->max_level, s->multiplier,
                s->base_atk_bonus, s->luk_bonus, s->hp_bonus, s->mp_bonus,
                s->str_bonus, s->dex_bonus, s->int_bonus, s->mp_cost,
                (int)s->type, (int)s->required_job,
                s->tier, s->prereq_id, s->is_passive, s->is_active);
    }

    // --- [ENCYCLOPEDIA] 섹션 ---
    fprintf(fp, "\n[ENCYCLOPEDIA]\n");
    fprintf(fp, "id,is_registered,kill_count\n");
    for (int i = 0; i < MAX_BOOK_ENTRIES; i++) {
        fprintf(fp, "%d,%d,%d\n",
                p->encyclopedia[i].id,
                p->encyclopedia[i].is_registered,
                p->encyclopedia[i].kill_count);
    }

    fclose(fp);
    return 1;
}

// --- CSV 따옴표 문자열 파싱 헬퍼 ---
// src가 "문자열" 형태면 따옴표를 제거하고 dst에 복사
static void parse_quoted_field(char* dst, int dst_size, const char** src_ptr) {
    const char* s = *src_ptr;
    
    if (*s == '"') {
        s++; // 여는 따옴표 건너뛰기
        int i = 0;
        while (*s && *s != '"' && i < dst_size - 1) {
            dst[i++] = *s++;
        }
        dst[i] = '\0';
        if (*s == '"') s++; // 닫는 따옴표 건너뛰기
        if (*s == ',') s++; // 구분자 건너뛰기
    } else {
        // 따옴표 없는 일반 필드
        int i = 0;
        while (*s && *s != ',' && *s != '\n' && *s != '\r' && i < dst_size - 1) {
            dst[i++] = *s++;
        }
        dst[i] = '\0';
        if (*s == ',') s++;
    }
    *src_ptr = s;
}

// --- 일반 필드 파싱 (숫자 등) ---
static int parse_int_field(const char** src_ptr) {
    const char* s = *src_ptr;
    char buf[64];
    int i = 0;
    while (*s && *s != ',' && *s != '\n' && *s != '\r' && i < 63) {
        buf[i++] = *s++;
    }
    buf[i] = '\0';
    if (*s == ',') s++;
    *src_ptr = s;
    return atoi(buf);
}

static float parse_float_field(const char** src_ptr) {
    const char* s = *src_ptr;
    char buf[64];
    int i = 0;
    while (*s && *s != ',' && *s != '\n' && *s != '\r' && i < 63) {
        buf[i++] = *s++;
    }
    buf[i] = '\0';
    if (*s == ',') s++;
    *src_ptr = s;
    return (float)atof(buf);
}

static long long parse_longlong_field(const char** src_ptr) {
    const char* s = *src_ptr;
    char buf[64];
    int i = 0;
    while (*s && *s != ',' && *s != '\n' && *s != '\r' && i < 63) {
        buf[i++] = *s++;
    }
    buf[i] = '\0';
    if (*s == ',') s++;
    *src_ptr = s;
    return atoll(buf);
}

// ============================================================
//  로드 (CSV 섹션 파일 → Player)
// ============================================================
int load_game(Player* p, const char* filepath) {
    FILE* fp = fopen(filepath, "r");
    if (!fp) {
        printf(RED "  [오류] 파일을 열 수 없습니다: %s\n" RESET, filepath);
        return 0;
    }

    char line[2048];
    char section[64] = "";

    // 스킬 시스템 초기화 (스킬 트리 데이터 로드)
    init_skill_system();

    while (fgets(line, sizeof(line), fp)) {
        // 개행 문자 제거
        line[strcspn(line, "\r\n")] = '\0';

        // 빈 줄 건너뛰기
        if (strlen(line) == 0) continue;

        // 섹션 헤더 감지
        if (line[0] == '[') {
            sscanf(line, "%63s", section);
            // 섹션 헤더 다음 줄은 컬럼 헤더이므로 건너뛰기
            if (fgets(line, sizeof(line), fp) == NULL) break;
            
            // [INVENTORY]와 [SKILLS]는 count= 줄이 먼저 옴
            if (strcmp(section, "[INVENTORY]") == 0 || strcmp(section, "[SKILLS]") == 0) {
                // count=N 파싱
                line[strcspn(line, "\r\n")] = '\0';
                if (strncmp(line, "count=", 6) == 0) {
                    int cnt = atoi(line + 6);
                    if (strcmp(section, "[INVENTORY]") == 0) p->inventory.count = cnt;
                    else p->skill_count = cnt;
                }
                // 실제 컬럼 헤더 줄 건너뛰기
                if (fgets(line, sizeof(line), fp) == NULL) break;
            }
            continue;
        }

        // --- [PLAYER] 데이터 파싱 ---
        if (strcmp(section, "[PLAYER]") == 0) {
            const char* ptr = line;
            parse_quoted_field(p->name, sizeof(p->name), &ptr);
            p->job = (JobType)parse_int_field(&ptr);
            p->level = parse_int_field(&ptr);
            p->hp = parse_int_field(&ptr);
            p->max_hp = parse_int_field(&ptr);
            p->mp = parse_int_field(&ptr);
            p->max_mp = parse_int_field(&ptr);
            p->str = parse_int_field(&ptr);
            p->dex = parse_int_field(&ptr);
            p->intel = parse_int_field(&ptr);
            p->luk = parse_int_field(&ptr);
            p->magic_atk = parse_int_field(&ptr);
            p->ied = parse_float_field(&ptr);
            p->boss_dmg = parse_float_field(&ptr);
            p->dmg_percent = parse_float_field(&ptr);
            p->exp = parse_int_field(&ptr);
            p->gold = parse_int_field(&ptr);
            p->stat_points = parse_int_field(&ptr);
            p->skill_points = parse_int_field(&ptr);
            p->weapon_tier = parse_int_field(&ptr);
            p->armor_tier = parse_int_field(&ptr);
            p->accessory_tier = parse_int_field(&ptr);
            p->c_weapon_tier = parse_int_field(&ptr);
            p->c_armor_tier = parse_int_field(&ptr);
            p->c_accessory_tier = parse_int_field(&ptr);
            p->weapon_dur = parse_int_field(&ptr);
            p->armor_dur = parse_int_field(&ptr);
            p->accessory_dur = parse_int_field(&ptr);
            p->c_weapon_dur = parse_int_field(&ptr);
            p->c_armor_dur = parse_int_field(&ptr);
            p->c_accessory_dur = parse_int_field(&ptr);
            p->combat_power = parse_int_field(&ptr);
            p->last_job_time = (time_t)parse_longlong_field(&ptr);
            p->job_count = parse_int_field(&ptr);
            section[0] = '\0'; // 1줄만 읽으면 끝
        }
        // --- [ABILITIES] 데이터 파싱 ---
        else if (strcmp(section, "[ABILITIES]") == 0) {
            static int ab_idx = 0;
            if (ab_idx < ABILITY_COUNT) {
                const char* ptr = line;
                p->abilities[ab_idx].rank = (AbilityRank)parse_int_field(&ptr);
                p->abilities[ab_idx].type = (AbilityType)parse_int_field(&ptr);
                p->abilities[ab_idx].value = parse_float_field(&ptr);
                p->ability_locked[ab_idx] = parse_int_field(&ptr);
                ab_idx++;
                if (ab_idx >= ABILITY_COUNT) ab_idx = 0; // 다음 로드를 위해 리셋
            }
        }
        // --- [INVENTORY] 데이터 파싱 ---
        else if (strcmp(section, "[INVENTORY]") == 0) {
            static int inv_idx = 0;
            if (inv_idx < p->inventory.count && inv_idx < MAX_INVENTORY_SIZE) {
                const char* ptr = line;
                parse_quoted_field(p->inventory.items[inv_idx].name, 
                                   sizeof(p->inventory.items[inv_idx].name), &ptr);
                parse_quoted_field(p->inventory.items[inv_idx].description,
                                   sizeof(p->inventory.items[inv_idx].description), &ptr);
                p->inventory.items[inv_idx].type = (ItemType)parse_int_field(&ptr);
                p->inventory.items[inv_idx].value = parse_int_field(&ptr);
                inv_idx++;
                if (inv_idx >= p->inventory.count) inv_idx = 0;
            }
        }
        // --- [SKILLS] 데이터 파싱 ---
        else if (strcmp(section, "[SKILLS]") == 0) {
            static int sk_idx = 0;
            if (sk_idx < p->skill_count && sk_idx < MAX_LEARNED_SKILLS) {
                Skill* s = &p->learned_skills[sk_idx];
                const char* ptr = line;
                s->id = parse_int_field(&ptr);
                parse_quoted_field(s->name, sizeof(s->name), &ptr);
                parse_quoted_field(s->description, sizeof(s->description), &ptr);
                s->level = parse_int_field(&ptr);
                s->max_level = parse_int_field(&ptr);
                s->multiplier = parse_float_field(&ptr);
                s->base_atk_bonus = parse_int_field(&ptr);
                s->luk_bonus = parse_int_field(&ptr);
                s->hp_bonus = parse_int_field(&ptr);
                s->mp_bonus = parse_int_field(&ptr);
                s->str_bonus = parse_int_field(&ptr);
                s->dex_bonus = parse_int_field(&ptr);
                s->int_bonus = parse_int_field(&ptr);
                s->mp_cost = parse_int_field(&ptr);
                s->type = (SkillType)parse_int_field(&ptr);
                s->required_job = (JobType)parse_int_field(&ptr);
                s->tier = parse_int_field(&ptr);
                s->prereq_id = parse_int_field(&ptr);
                s->is_passive = parse_int_field(&ptr);
                s->is_active = parse_int_field(&ptr);
                sk_idx++;
                if (sk_idx >= p->skill_count) sk_idx = 0;
            }
        }
        // --- [ENCYCLOPEDIA] 데이터 파싱 ---
        else if (strcmp(section, "[ENCYCLOPEDIA]") == 0) {
            static int enc_idx = 0;
            if (enc_idx < MAX_BOOK_ENTRIES) {
                const char* ptr = line;
                p->encyclopedia[enc_idx].id = parse_int_field(&ptr);
                p->encyclopedia[enc_idx].is_registered = parse_int_field(&ptr);
                p->encyclopedia[enc_idx].kill_count = parse_int_field(&ptr);
                enc_idx++;
                if (enc_idx >= MAX_BOOK_ENTRIES) enc_idx = 0;
            }
        }
    }

    fclose(fp);
    update_combat_power(p);
    return 1;
}

// ============================================================
//  즉시 저장 (캐릭터 이름 기반 자동 파일명)
// ============================================================
void quick_save(Player* p) {
    ensure_save_dir();

    char filepath[MAX_FILENAME_LEN];
    snprintf(filepath, sizeof(filepath), "%s/%s.csv", SAVE_DIR, p->name);

    if (save_game(p, filepath)) {
        printf("\n  " GREEN "✅ 저장 완료!" RESET " → %s (Lv.%d %s)\n",
               filepath, p->level, get_job_name(p->job));
    } else {
        printf("\n  " RED "❌ 저장 실패!" RESET "\n");
    }

    wait_for_enter();
}

// ============================================================
//  불러오기 메뉴 UI
// ============================================================
int show_load_menu(Player* p) {
    clear_screen();
    int box_width = 80;

    print_divider(box_width, CYAN);
    print_box_line("  📂 저장 파일 불러오기", box_width, CYAN);
    print_divider(box_width, CYAN);
    printf("\n");
    
    // saves 디렉터리에서 .csv 파일 목록 열거
    ensure_save_dir();
    
    char filenames[MAX_SAVE_SLOTS][128];
    int file_count = 0;

#ifdef _WIN32
    // Windows: _findfirst / _findnext
    struct _finddata_t fileinfo;
    intptr_t handle;
    char search_path[MAX_FILENAME_LEN];
    sprintf(search_path, "%s/*.csv", SAVE_DIR);
    
    handle = _findfirst(search_path, &fileinfo);
    if (handle != -1) {
        do {
            if (file_count < MAX_SAVE_SLOTS) {
                strncpy(filenames[file_count], fileinfo.name, 127);
                filenames[file_count][127] = '\0';
                file_count++;
            }
        } while (_findnext(handle, &fileinfo) == 0);
        _findclose(handle);
    }
#else
    // POSIX: opendir / readdir
    DIR* dir = opendir(SAVE_DIR);
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL && file_count < MAX_SAVE_SLOTS) {
            // .csv 확장자 필터링
            const char* ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".csv") == 0) {
                strncpy(filenames[file_count], entry->d_name, 127);
                filenames[file_count][127] = '\0';
                file_count++;
            }
        }
        closedir(dir);
    }
#endif

    // 파일 미리보기 출력
    for (int i = 0; i < file_count; i++) {
        char preview_path[512];
        snprintf(preview_path, sizeof(preview_path), "%s/%s", SAVE_DIR, filenames[i]);
        FILE* pf = fopen(preview_path, "r");
        char preview_name[50] = "???";
        char preview_info[128] = "";
        if (pf) {
            char buf[2048];
            // [PLAYER] 헤더 건너뛰기
            if (fgets(buf, sizeof(buf), pf)) {
                // 컬럼 헤더 건너뛰기
                if (fgets(buf, sizeof(buf), pf)) {
                    // 데이터 줄 읽기
                    if (fgets(buf, sizeof(buf), pf)) {
                        buf[strcspn(buf, "\r\n")] = '\0';
                        const char* ptr = buf;
                        char pname[50];
                        parse_quoted_field(pname, sizeof(pname), &ptr);
                        int pjob = parse_int_field(&ptr);
                        int plevel = parse_int_field(&ptr);
                        strncpy(preview_name, pname, 49);
                        sprintf(preview_info, "Lv.%d %s", plevel, get_job_name((JobType)pjob));
                    }
                }
            }
            fclose(pf);
        }
        printf("  " GREEN "%2d." RESET " %-30s  " CYAN "[%s - %s]" RESET "\n",
               i + 1, filenames[i], preview_name, preview_info);
    }
    
    if (file_count == 0) {
        printf("  " YELLOW "(저장된 파일이 없습니다)" RESET "\n");
        printf("\n");
        print_divider(box_width, CYAN);
        printf("\n  새로운 모험을 시작하세요!\n");
        wait_for_enter();
        return 0;
    }
    
    printf("\n");
    print_divider(box_width, CYAN);
    printf("  불러올 파일 번호를 선택하세요 (취소: 0): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    
    if (choice <= 0 || choice > file_count) {
        printf("\n  " YELLOW "불러오기가 취소되었습니다." RESET "\n");
        wait_for_enter();
        return 0;
    }
    
    char filepath[MAX_FILENAME_LEN];
    sprintf(filepath, "%s/%s", SAVE_DIR, filenames[choice - 1]);
    
    if (load_game(p, filepath)) {
        printf("\n  " GREEN "✅ 불러오기 완료!" RESET " → %s (Lv.%d %s)\n", 
               p->name, p->level, get_job_name(p->job));
        wait_for_enter();
        return 1;
    } else {
        printf("\n  " RED "❌ 불러오기 실패!" RESET "\n");
        wait_for_enter();
        return 0;
    }
}
