#include <stdlib.h>
#include <time.h>
#include "utils.h"

int get_random_range(int min, int max) {
    // Seed는 보통 main에서 한 번만 호출하는 것이 좋지만, 
    // 여기서는 간단히 구현하거나 main에서 호출하도록 안내합니다.
    return rand() % (max - min + 1) + min;
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
