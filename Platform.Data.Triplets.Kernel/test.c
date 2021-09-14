#include "Link.h"
#include "PersistentMemoryManager.h"
#include "PersistentMemoryManager.c"
#include <stdio.h>
#include <stdlib.h>

int main() {

    struct RawDB* db = malloc(sizeof(struct RawDB));

    printf("Startup.\n");

    printf("!!!!!!!!!!!!!!%d", OpenLinks(db, "db.links"));

    printf("Database opened.\n");

    link_index isA = CreateLink(db, itself, itself, itself);
    link_index isNotA = CreateLink(db, itself, itself, isA);
    link_index link = CreateLink(db, itself, isA, itself);
    link_index thing = CreateLink(db, itself, isNotA, link);

    UpdateLink(db, isA, isA, isA, link); // После этого минимальное ядро системы можно считать сформированным
    
    DeleteLink(db, isA); // Одна эта операция удалит все 4 связи
    DeleteLink(db, thing);
    
    printf("Test ok.\n");
    
    CloseLinks(db);
    
    printf("Database closed.\n");
    
    return 0;
}
