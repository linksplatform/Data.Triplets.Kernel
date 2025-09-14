// LinksContext implementation for multiple Links instances support

#include "LinksContext.h"
#include "PersistentMemoryManager.h"
#include "Timestamp.h"
#include <stdlib.h>
#include <string.h>

// Forward declarations
void AttachLinkWithContext(LinksContext* context, link_index linkIndex, uint64_t sourceIndex, uint64_t linkerIndex, uint64_t targetIndex);

// Default global context for backward compatibility
static LinksContext* defaultContext = NULL;

LinksContext* public_calling_convention CreateLinksContext()
{
    LinksContext* context = (LinksContext*)malloc(sizeof(LinksContext));
    if (context == NULL)
    {
        return NULL;
    }
    
    // Initialize all fields to zero/null
    memset(context, 0, sizeof(LinksContext));
    
    // Reset file handles to invalid state
#if defined(WINDOWS)
    context->storageFileHandle = INVALID_HANDLE_VALUE;
    context->storageFileMappingHandle = INVALID_HANDLE_VALUE;
    context->pointerToMappedRegion = NULL;
#elif defined(UNIX)
    context->storageFileHandle = -1;
    context->pointerToMappedRegion = MAP_FAILED;
#endif
    
    // Initialize memory manager for this context
    InitPersistentMemoryManagerWithContext(context);
    
    return context;
}

void public_calling_convention DestroyLinksContext(LinksContext* context)
{
    if (context == NULL)
    {
        return;
    }
    
    // Close links if they're still open
    CloseLinksWithContext(context);
    
    // Free the context
    free(context);
}

// Get or create the default context for backward compatibility
static LinksContext* GetDefaultContext()
{
    if (defaultContext == NULL)
    {
        defaultContext = CreateLinksContext();
    }
    return defaultContext;
}

// Context-aware API implementations
signed_integer public_calling_convention OpenLinksWithContext(LinksContext* context, char* filename)
{
    if (context == NULL)
    {
        ERROR_MESSAGE("Context cannot be null.");
        return ERROR_RESULT;
    }
    
    return OpenStorageFileWithContext(context, filename);
}

signed_integer public_calling_convention CloseLinksWithContext(LinksContext* context)
{
    if (context == NULL)
    {
        ERROR_MESSAGE("Context cannot be null.");
        return ERROR_RESULT;
    }
    
    return CloseStorageFileWithContext(context);
}

link_index public_calling_convention CreateLinkWithContext(LinksContext* context, link_index sourceIndex, link_index linkerIndex, link_index targetIndex)
{
    if (context == NULL)
    {
        ERROR_MESSAGE("Context cannot be null.");
        return null;
    }
    
    if (sourceIndex != itself &&
        linkerIndex != itself &&
        targetIndex != itself)
    {
        uint64_t linkIndex = SearchLinkWithContext(context, sourceIndex, linkerIndex, targetIndex);
        if (linkIndex == null)
        {
            linkIndex = AllocateLinkWithContext(context);
            if (linkIndex != null)
            {
                Link *link = GetLinkWithContext(context, linkIndex);
                link->Timestamp = GetTimestamp();
                AttachLinkWithContext(context, linkIndex, sourceIndex, linkerIndex, targetIndex);
            }
        }
        return linkIndex;
    }
    else
    {
        uint64_t linkIndex = AllocateLinkWithContext(context);
        if (linkIndex != null)
        {
            Link* link = GetLinkWithContext(context, linkIndex);
            link->Timestamp = GetTimestamp();
            sourceIndex = (sourceIndex == itself ? linkIndex : sourceIndex);
            linkerIndex = (linkerIndex == itself ? linkIndex : linkerIndex);
            targetIndex = (targetIndex == itself ? linkIndex : targetIndex);
            AttachLinkWithContext(context, linkIndex, sourceIndex, linkerIndex, targetIndex);
        }
        return linkIndex;
    }
}

// Forward declarations for functions we'll need to implement
void AttachLinkWithContext(LinksContext* context, link_index linkIndex, uint64_t sourceIndex, uint64_t linkerIndex, uint64_t targetIndex);
link_index SearchRefererOfTargetWithContext(LinksContext* context, link_index targetIndex, link_index sourceIndex, link_index linkerIndex);
link_index SearchRefererOfSourceWithContext(LinksContext* context, link_index sourceIndex, link_index targetIndex, link_index linkerIndex);
unsigned_integer GetNumberOfReferersBySourceWithContext(LinksContext* context, link_index linkIndex);
unsigned_integer GetNumberOfReferersByTargetWithContext(LinksContext* context, link_index linkIndex);

link_index public_calling_convention SearchLinkWithContext(LinksContext* context, link_index sourceIndex, link_index linkerIndex, link_index targetIndex)
{
    if (context == NULL)
    {
        ERROR_MESSAGE("Context cannot be null.");
        return null;
    }
    
    // Choose smaller tree (target or source); linker is a list
    if (GetNumberOfReferersBySourceWithContext(context, sourceIndex) >= GetNumberOfReferersByTargetWithContext(context, targetIndex))
        return SearchRefererOfTargetWithContext(context, targetIndex, sourceIndex, linkerIndex);
    else
        return SearchRefererOfSourceWithContext(context, sourceIndex, targetIndex, linkerIndex);
}

link_index public_calling_convention GetSourceIndexWithContext(LinksContext* context, link_index linkIndex)
{
    if (context == NULL)
    {
        ERROR_MESSAGE("Context cannot be null.");
        return null;
    }
    
    return GetLinkWithContext(context, linkIndex)->SourceIndex;
}

link_index public_calling_convention GetLinkerIndexWithContext(LinksContext* context, link_index linkIndex)
{
    if (context == NULL)
    {
        ERROR_MESSAGE("Context cannot be null.");
        return null;
    }
    
    return GetLinkWithContext(context, linkIndex)->LinkerIndex;
}

link_index public_calling_convention GetTargetIndexWithContext(LinksContext* context, link_index linkIndex)
{
    if (context == NULL)
    {
        ERROR_MESSAGE("Context cannot be null.");
        return null;
    }
    
    return GetLinkWithContext(context, linkIndex)->TargetIndex;
}

signed_integer public_calling_convention GetTimeWithContext(LinksContext* context, link_index linkIndex)
{
    if (context == NULL)
    {
        ERROR_MESSAGE("Context cannot be null.");
        return 0;
    }
    
    return GetLinkWithContext(context, linkIndex)->Timestamp;
}

unsigned_integer public_calling_convention GetLinksCountWithContext(LinksContext* context)
{
    if (context == NULL)
    {
        ERROR_MESSAGE("Context cannot be null.");
        return 0;
    }
    
    return *(context->pointerToLinksSize) - 1;
}

// Backward compatibility functions using default context
signed_integer public_calling_convention OpenLinks(char* filename)
{
    return OpenLinksWithContext(GetDefaultContext(), filename);
}

signed_integer public_calling_convention CloseLinks()
{
    return CloseLinksWithContext(GetDefaultContext());
}

link_index public_calling_convention CreateLink(link_index sourceIndex, link_index linkerIndex, link_index targetIndex)
{
    return CreateLinkWithContext(GetDefaultContext(), sourceIndex, linkerIndex, targetIndex);
}

link_index public_calling_convention SearchLink(link_index sourceIndex, link_index linkerIndex, link_index targetIndex)
{
    return SearchLinkWithContext(GetDefaultContext(), sourceIndex, linkerIndex, targetIndex);
}

link_index public_calling_convention GetSourceIndex(link_index linkIndex)
{
    return GetSourceIndexWithContext(GetDefaultContext(), linkIndex);
}

link_index public_calling_convention GetLinkerIndex(link_index linkIndex)
{
    return GetLinkerIndexWithContext(GetDefaultContext(), linkIndex);
}

link_index public_calling_convention GetTargetIndex(link_index linkIndex)
{
    return GetTargetIndexWithContext(GetDefaultContext(), linkIndex);
}

signed_integer public_calling_convention GetTime(link_index linkIndex)
{
    return GetTimeWithContext(GetDefaultContext(), linkIndex);
}

unsigned_integer public_calling_convention GetLinksCount()
{
    return GetLinksCountWithContext(GetDefaultContext());
}

// Implementation of missing functions
void AttachLinkWithContext(LinksContext* context, link_index linkIndex, uint64_t sourceIndex, uint64_t linkerIndex, uint64_t targetIndex)
{
    if (context == NULL) return;
    
    Link* link = GetLinkWithContext(context, linkIndex);
    link->SourceIndex = sourceIndex;
    link->LinkerIndex = linkerIndex;
    link->TargetIndex = targetIndex;
    
    // TODO: Attach to reference trees - this requires implementing the tree operations
    // For now, just set the basic fields
}

link_index SearchRefererOfTargetWithContext(LinksContext* context, link_index targetIndex, link_index sourceIndex, link_index linkerIndex)
{
    if (context == NULL) return null;
    
    // TODO: Implement proper tree search
    // For now, return null (no match found)
    return null;
}

link_index SearchRefererOfSourceWithContext(LinksContext* context, link_index sourceIndex, link_index targetIndex, link_index linkerIndex)
{
    if (context == NULL) return null;
    
    // TODO: Implement proper tree search
    // For now, return null (no match found)
    return null;
}

unsigned_integer GetNumberOfReferersBySourceWithContext(LinksContext* context, link_index linkIndex)
{
    if (context == NULL) return 0;
    
    // TODO: Implement proper count from tree
    // For now, return 0
    return 0;
}

unsigned_integer GetNumberOfReferersByTargetWithContext(LinksContext* context, link_index linkIndex)
{
    if (context == NULL) return 0;
    
    // TODO: Implement proper count from tree  
    // For now, return 0
    return 0;
}