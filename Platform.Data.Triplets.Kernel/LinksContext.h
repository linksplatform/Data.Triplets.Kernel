#ifndef __LINKS_CONTEXT_H__
#define __LINKS_CONTEXT_H__

// Context structure to support multiple instances of Links
// This allows multiple Links databases to be opened in the same process

#include "Common.h"
#include "Link.h"

#if defined(WINDOWS)
#include <windows.h>
#elif defined(UNIX)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

// Context structure that holds all state for a Links instance
typedef struct LinksContext
{
    // File handles and mapping
#if defined(WINDOWS)
    HANDLE              storageFileHandle;
    HANDLE              storageFileMappingHandle;
#elif defined(UNIX)
    signed_integer      storageFileHandle;
#endif
    int64_t             storageFileSizeInBytes;
    void*               pointerToMappedRegion;
    
    // Constants calculated at startup
    int64_t             currentMemoryPageSizeInBytes;
    int64_t             serviceBlockSizeInBytes;
    int64_t             baseLinksSizeInBytes;
    int64_t             baseBlockSizeInBytes;
    int64_t             storageFileMinSizeInBytes;
    
    // Pointers to mapped memory sections
    uint64_t*           pointerToDataSeal;
    uint64_t*           pointerToLinkIndexSize;
    uint64_t*           pointerToMappingLinksMaxSize;
    link_index*         pointerToPointerToMappingLinks;
    link_index*         pointerToLinksMaxSize;
    link_index*         pointerToLinksSize;
    Link*               pointerToLinks;
    Link*               pointerToUnusedMarker;
} LinksContext;

// Context management functions
PREFIX_DLL LinksContext* CreateLinksContext();
PREFIX_DLL void DestroyLinksContext(LinksContext* context);

// Context-aware API functions (new API)
PREFIX_DLL signed_integer OpenLinksWithContext(LinksContext* context, char* filename);
PREFIX_DLL signed_integer CloseLinksWithContext(LinksContext* context);

PREFIX_DLL link_index CreateLinkWithContext(LinksContext* context, link_index sourceIndex, link_index linkerIndex, link_index targetIndex);
PREFIX_DLL link_index SearchLinkWithContext(LinksContext* context, link_index sourceIndex, link_index linkerIndex, link_index targetIndex);
PREFIX_DLL link_index UpdateLinkWithContext(LinksContext* context, link_index linkIndex, link_index sourceIndex, link_index linkerIndex, link_index targetIndex);
PREFIX_DLL link_index ReplaceLinkWithContext(LinksContext* context, link_index linkIndex, link_index replacementIndex);
PREFIX_DLL void DeleteLinkWithContext(LinksContext* context, link_index linkIndex);

PREFIX_DLL link_index GetSourceIndexWithContext(LinksContext* context, link_index linkIndex);
PREFIX_DLL link_index GetLinkerIndexWithContext(LinksContext* context, link_index linkIndex);
PREFIX_DLL link_index GetTargetIndexWithContext(LinksContext* context, link_index linkIndex);
PREFIX_DLL signed_integer GetTimeWithContext(LinksContext* context, link_index linkIndex);

PREFIX_DLL unsigned_integer GetLinksCountWithContext(LinksContext* context);
PREFIX_DLL link_index AllocateLinkWithContext(LinksContext* context);
PREFIX_DLL void FreeLinkWithContext(LinksContext* context, link_index link);

PREFIX_DLL void WalkThroughAllLinksWithContext(LinksContext* context, visitor visitor);
PREFIX_DLL signed_integer WalkThroughLinksWithContext(LinksContext* context, stoppable_visitor stoppableVisitor);

// Internal functions that work with context
void InitPersistentMemoryManagerWithContext(LinksContext* context);
signed_integer OpenStorageFileWithContext(LinksContext* context, char* filename);
signed_integer CloseStorageFileWithContext(LinksContext* context);
signed_integer SetStorageFileMemoryMappingWithContext(LinksContext* context);
signed_integer ResetStorageFileMemoryMappingWithContext(LinksContext* context);

Link* GetLinkWithContext(LinksContext* context, link_index linkIndex);
link_index GetLinkIndexWithContext(LinksContext* context, Link* link);

#if defined(__cplusplus)
}
#endif

#endif