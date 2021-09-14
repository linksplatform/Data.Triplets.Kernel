#include "Link.h"
#include "PersistentMemoryManager.h"
#include <stdio.h>

int main(RawDB* db, ) {
    
    printf("Startup.\n");
    
    OpenLinks("db.links");
    
    printf("Database opened.\n");
    
    link_index isA = CreateLink(RawDB* db, itself, itself, itself);
    link_index isNotA = CreateLink(RawDB* db, itself, itself, isA);
    link_index link = CreateLink(RawDB* db, itself, isA, itself);
    link_index thing = CreateLink(RawDB* db, itself, isNotA, link);
    
    UpdateLink(RawDB* db, isA, isA, isA, link); // После этого минимальное ядро системы можно считать сформированным
    
    DeleteLink(RawDB* db, isA); // Одна эта операция удалит все 4 связи
    DeleteLink(RawDB* db, thing);
    
    printf("Test ok.\n");
    
    CloseLinks(RawDB* db, );
    
    printf("Database closed.\n");
    
    return 0;
}
