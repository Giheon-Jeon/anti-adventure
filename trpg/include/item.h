#ifndef ITEM_H
#define ITEM_H

#define MAX_INVENTORY_SIZE 20

typedef enum {
    ITEM_TYPE_TROPHY,    // 전리품
    ITEM_TYPE_CONSUMABLE // 소비 아이템 (물약 등)
} ItemType;

typedef struct {
    char name[50];
    char description[100];
    ItemType type;
    int value; // 상점 판매가 등
} Item;

typedef struct {
    Item items[MAX_INVENTORY_SIZE];
    int count;
} Inventory;

#endif
