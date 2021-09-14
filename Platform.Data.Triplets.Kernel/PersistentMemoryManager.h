#ifndef __LINKS_PERSISTENT_MEMORY_MANAGER_H__
#define __LINKS_PERSISTENT_MEMORY_MANAGER_H__

// Persistent on drive memory manager (Менеджер хранимой на диске памяти).

#include "Common.h"
#include "Link.h"

#if defined(__cplusplus)
extern "C" {
#endif

    void InitPersistentMemoryManager(RawDB* db);
    
    signed_integer OpenStorageFile(RawDB* db, const char* filename);
    signed_integer CloseStorageFile(RawDB* db);
    signed_integer EnlargeStorageFile(RawDB* db);
    signed_integer ShrinkStorageFile(RawDB* db);
    signed_integer SetStorageFileMemoryMapping(RawDB* db);
    signed_integer ResetStorageFileMemoryMapping(RawDB* db);
    
    PREFIX_DLL signed_integer OpenLinks(RawDB* db, const char* filename);
    PREFIX_DLL signed_integer CloseLinks(RawDB* db);
    
    PREFIX_DLL link_index GetMappedLink(RawDB* db, signed_integer mappedIndex);
    PREFIX_DLL void SetMappedLink(RawDB* db, signed_integer mappedIndex, link_index linkIndex);

    PREFIX_DLL void WalkThroughAllLinks(RawDB* db, visitor visitor);
    PREFIX_DLL signed_integer WalkThroughLinks(RawDB* db, stoppable_visitor stoppableVisitor);

    PREFIX_DLL unsigned_integer GetLinksCount(RawDB* db);

    // Exported only for Tests, Unsafe to use directly (use Create/Update/Delete instead)
    PREFIX_DLL link_index AllocateLink(RawDB* db);
    PREFIX_DLL void FreeLink(RawDB* db, link_index link);

    Link* GetLink(RawDB* db, link_index linkIndex);
    link_index GetLinkIndex(RawDB* db, Link* link);

#if defined(__cplusplus)
}
#endif

#define LINKS_DATA_SEAL_64BIT 0x810118808100180
// Binary:
// 0000100000010000
// 0001000110001000
// 0000100000010000
// 0000000110000000‬
#endif
