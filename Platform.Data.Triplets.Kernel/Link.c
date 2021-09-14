
// Высокоуровневая логика работы с "линками".

#include "Common.h"
#include "Timestamp.h"
#include "Link.h"
#include "PersistentMemoryManager.h"
#include "SizeBalancedTree.h"
#include "LinkLowLevel.h"

//DefineAllReferersTreeMethods(Source)
//DefineAllReferersTreeMethods(Linker)
//DefineAllReferersTreeMethods(Target)
//DefineAllSearchMethods()

link_index SearchRefererOfSource(RawDB* db, link_index linkIndex, link_index refererTargetIndex,
        link_index refererLinkerIndex) {
    link_index currentNode = GetLink(db, linkIndex)->BySourceRootIndex;
    while (currentNode)
        if ((GetLink(db, currentNode)->LinkerIndex > (refererLinkerIndex)
                || (GetLink(db, currentNode)->LinkerIndex == (refererLinkerIndex)
                        && GetLink(db, currentNode)->TargetIndex > (refererTargetIndex))))
            currentNode = GetLink(db, currentNode)->BySourceLeftIndex;
        else if ((GetLink(db, currentNode)->LinkerIndex < (refererLinkerIndex)
                || (GetLink(db, currentNode)->LinkerIndex == (refererLinkerIndex)
                        && GetLink(db, currentNode)->TargetIndex < (refererTargetIndex))))
            currentNode = GetLink(db, currentNode)->BySourceRightIndex;
        else return currentNode;
    return 0LL;
}
link_index SearchRefererOfLinker(RawDB* db, link_index linkIndex, link_index refererSourceIndex, link_index refererTargetIndex) {
    {
        link_index firstElementIndex = GetLink(db, linkIndex)->ByTargetRootIndex;
        if (firstElementIndex != 0LL) {
            link_index referer = firstElementIndex;
            do {
                if (GetLink(db, referer)->SourceIndex == refererSourceIndex
                        && GetLink(db, referer)->TargetIndex == refererTargetIndex)
                    return referer;
                referer = GetLink(db, referer)->ByTargetRightIndex;
            }
            while (referer != firstElementIndex);
        }
    };
    return 0LL;
}
link_index SearchRefererOfTarget(RawDB* db, link_index linkIndex, link_index refererSourceIndex,
        link_index refererLinkerIndex) {
    link_index currentNode = GetLink(db, linkIndex)->ByTargetRootIndex;
    while (currentNode)
        if ((GetLink(db, currentNode)->LinkerIndex > (refererLinkerIndex)
                || (GetLink(db, currentNode)->LinkerIndex == (refererLinkerIndex)
                        && GetLink(db, currentNode)->SourceIndex > (refererSourceIndex))))
            currentNode = GetLink(db, currentNode)->ByTargetLeftIndex;
        else if ((GetLink(db, currentNode)->LinkerIndex < (refererLinkerIndex)
                || (GetLink(db, currentNode)->LinkerIndex == (refererLinkerIndex)
                        && GetLink(db, currentNode)->SourceIndex < (refererSourceIndex))))
            currentNode = GetLink(db, currentNode)->ByTargetRightIndex;
        else return currentNode;
    return 0LL;
}

link_index public_calling_convention GetSourceIndex(RawDB* db, link_index linkIndex)
{
    return GetLink(db, linkIndex)->SourceIndex;
}

link_index public_calling_convention GetLinkerIndex(RawDB* db, link_index linkIndex)
{
    return GetLink(db, linkIndex)->LinkerIndex;
}

link_index public_calling_convention GetTargetIndex(RawDB* db, link_index linkIndex)
{
    return GetLink(db, linkIndex)->TargetIndex;
}

signed_integer public_calling_convention GetTime(RawDB* db, link_index linkIndex)
{
    return GetLink(db, linkIndex)->Timestamp;
}

link_index public_calling_convention CreateLink(RawDB* db, link_index sourceIndex, link_index linkerIndex, link_index targetIndex)
{
    if (sourceIndex != itself &&
        linkerIndex != itself &&
        targetIndex != itself)
    {
        uint64_t linkIndex = SearchLink(db, sourceIndex, linkerIndex, targetIndex);
        if (linkIndex == null)
        {
            linkIndex = AllocateLink(db);
            if (linkIndex != null)
            {
                Link *link = GetLink(db, linkIndex);
                link->Timestamp = GetTimestamp();
                AttachLink(db, linkIndex, sourceIndex, linkerIndex, targetIndex);
            }
        }
        return linkIndex;
    }
    else
    {
        uint64_t linkIndex = AllocateLink(db);
        if (linkIndex != null)
        {
            Link* link = GetLink(db, linkIndex);
            link->Timestamp = GetTimestamp();
            sourceIndex = (sourceIndex == itself ? linkIndex : sourceIndex);
            linkerIndex = (linkerIndex == itself ? linkIndex : linkerIndex);
            targetIndex = (targetIndex == itself ? linkIndex : targetIndex);
            AttachLink(db, linkIndex, sourceIndex, linkerIndex, targetIndex);
        }
        return linkIndex;
    }
}

link_index public_calling_convention SearchLink(RawDB* db, link_index sourceIndex, link_index linkerIndex, link_index targetIndex)
{
    // смотря, какое дерево меньше (target или source); по linker - список
    if (GetNumberOfReferersBySource(sourceIndex) >= GetNumberOfReferersByTarget(targetIndex))
        return SearchRefererOfTarget(db, targetIndex, sourceIndex, linkerIndex);
    else
        return SearchRefererOfSource(db, sourceIndex, targetIndex, linkerIndex);
}

link_index public_calling_convention ReplaceLink(RawDB* db, link_index linkIndex, link_index replacementIndex)
{
    Link *link = GetLink(db, linkIndex);
    Link *replacement = GetLink(db, replacementIndex);

    if (linkIndex != replacementIndex)
    {
        uint64_t firstRefererBySourceIndex = link->BySourceRootIndex;
        uint64_t firstRefererByLinkerIndex = link->ByLinkerRootIndex;
        uint64_t firstRefererByTargetIndex = link->ByTargetRootIndex;

        while (firstRefererBySourceIndex != null)
        {
            UpdateLink(
                db,
                firstRefererBySourceIndex,
                replacementIndex,
                GetLink(db, firstRefererBySourceIndex)->LinkerIndex,
                GetLink(db, firstRefererBySourceIndex)->TargetIndex
            );
            firstRefererBySourceIndex = link->BySourceRootIndex;
        }

        while (firstRefererByLinkerIndex != null)
        {
            UpdateLink(
                db,
                firstRefererByLinkerIndex,
                GetLink(db, firstRefererByLinkerIndex)->SourceIndex,
                replacementIndex,
                GetLink(db, firstRefererByLinkerIndex)->TargetIndex
            );
            firstRefererByLinkerIndex = link->ByLinkerRootIndex;
        }

        while (firstRefererByTargetIndex != null)
        {
            UpdateLink(
                db,
                firstRefererByTargetIndex,
                GetLink(db, firstRefererByTargetIndex)->SourceIndex,
                GetLink(db, firstRefererByTargetIndex)->LinkerIndex,
                replacementIndex
            );
            firstRefererByTargetIndex = link->ByTargetRootIndex;
        }

        DeleteLink(db, linkIndex);

        replacement->Timestamp = GetTimestamp();
    }
    return replacementIndex;
}

link_index public_calling_convention UpdateLink(RawDB* db, link_index linkIndex, link_index sourceIndex, link_index linkerIndex, link_index targetIndex)
{
    Link *link = GetLink(db, linkIndex);
    if (link->SourceIndex == sourceIndex && link->LinkerIndex == linkerIndex && link->TargetIndex == targetIndex)
        return linkIndex;

    if (sourceIndex != itself && linkerIndex != itself && targetIndex != itself)
    {
        uint64_t existingLinkIndex = SearchLink(db, sourceIndex, linkerIndex, targetIndex);
        if (existingLinkIndex == null)
        {
            DetachLink(db, linkIndex);
            AttachLink(db, linkIndex, sourceIndex, linkerIndex, targetIndex);

            link->Timestamp = GetTimestamp();

            return linkIndex;
        }
        else
        {
            return ReplaceLink(db, linkIndex, existingLinkIndex);
        }
    }
    else
    {
        sourceIndex = (sourceIndex == itself ? linkIndex : sourceIndex);
        linkerIndex = (linkerIndex == itself ? linkIndex : linkerIndex);
        targetIndex = (targetIndex == itself ? linkIndex : targetIndex);

        DetachLink(db, linkIndex);
        AttachLink(db, linkIndex, sourceIndex, linkerIndex, targetIndex);

        link->Timestamp = GetTimestamp();

        return linkIndex;
    }
}

void public_calling_convention DeleteLink(RawDB* db, link_index linkIndex)
{
    if (linkIndex == null) return;

    DetachLink(db, linkIndex);

    Link *link = GetLink(db, linkIndex);
    link->Timestamp = 0;

    while (link->BySourceRootIndex != null) DeleteLink(db, link->BySourceRootIndex);
    while (link->ByLinkerRootIndex != null) DeleteLink(db, link->ByLinkerRootIndex);
    while (link->ByTargetRootIndex != null) DeleteLink(db, link->ByTargetRootIndex);

    FreeLink(db, linkIndex);
}

link_index public_calling_convention GetFirstRefererBySourceIndex(RawDB* db, link_index linkIndex)
{
    return GetLink(db, linkIndex)->BySourceRootIndex;
}

link_index public_calling_convention GetFirstRefererByLinkerIndex(RawDB* db, link_index linkIndex)
{
    return GetLink(db, linkIndex)->ByLinkerRootIndex;
}

link_index public_calling_convention GetFirstRefererByTargetIndex(RawDB* db, link_index linkIndex)
{
    return GetLink(db, linkIndex)->ByTargetRootIndex;
}

unsigned_integer public_calling_convention GetLinkNumberOfReferersBySource(RawDB* db, link_index linkIndex) { return GetNumberOfReferersBySource(linkIndex); }
unsigned_integer public_calling_convention GetLinkNumberOfReferersByLinker(RawDB* db, link_index linkIndex) { return GetNumberOfReferersByLinker(linkIndex); }
unsigned_integer public_calling_convention GetLinkNumberOfReferersByTarget(RawDB* db, link_index linkIndex) { return GetNumberOfReferersByTarget(linkIndex); }

void WalkThroughAllReferersBySourceCore(RawDB* db, link_index rootIndex, visitor visitor)
{
    if (rootIndex != null)
    {
        Link* root = GetLink(db, rootIndex);
        WalkThroughAllReferersBySourceCore(db, root->BySourceLeftIndex, visitor);
        visitor(rootIndex);
        WalkThroughAllReferersBySourceCore(db, root->BySourceRightIndex, visitor);
    }
}

int WalkThroughReferersBySourceCore(RawDB* db, link_index rootIndex, stoppable_visitor stoppableVisitor)
{
    if (rootIndex != null)
    {
        Link* root = GetLink(db, rootIndex);
        if (!WalkThroughReferersBySourceCore(db, root->BySourceLeftIndex, stoppableVisitor)) return false;
        if (!stoppableVisitor(rootIndex)) return false;
        if (!WalkThroughReferersBySourceCore(db, root->BySourceRightIndex, stoppableVisitor)) return false;
    }
    return true;
}

void public_calling_convention WalkThroughAllReferersBySource(RawDB* db, link_index rootIndex, visitor visitor)
{
    if (rootIndex != null) WalkThroughAllReferersBySourceCore(db, GetLink(db, rootIndex)->BySourceRootIndex, visitor);
}

//void public_calling_convention WalkThroughAllReferersBySource1(link_index rootIndex, visitor visitor)
//{
//    BeginWalkThroughtTreeOfReferersBySource(RawDB* db, element, rootIndex);
//    visitor(RawDB* db, element);
//    EndWalkThroughtTreeOfReferersBySource(RawDB* db, element);
//}

signed_integer public_calling_convention WalkThroughReferersBySource(RawDB* db, link_index rootIndex, stoppable_visitor stoppableVisitor)
{
    if (rootIndex != null) return WalkThroughReferersBySourceCore(db, GetLink(db, rootIndex)->BySourceRootIndex, stoppableVisitor);
    else return true;
}

void public_calling_convention WalkThroughAllReferersByLinker(RawDB* db, link_index rootIndex, visitor visitor)
{
    if (rootIndex != null)
    {
        BeginWalkThroughReferersByLinker(element, rootIndex)
        {
            visitor(element);
        }
        EndWalkThroughReferersByLinker(element);
    }
}

signed_integer public_calling_convention WalkThroughReferersByLinker(RawDB* db, link_index rootIndex, stoppable_visitor stoppableVisitor)
{
    if (rootIndex != null)
    {
        BeginWalkThroughReferersByLinker(element, rootIndex)
        {
            if (!stoppableVisitor(element)) return false;
        }
        EndWalkThroughReferersByLinker(element);
    }
    return true;
}

void WalkThroughAllReferersByTargetCore(RawDB* db, link_index rootIndex, visitor visitor)
{
    if (rootIndex != null)
    {
        Link* root = GetLink(db, rootIndex);
        WalkThroughAllReferersByTargetCore(db, root->ByTargetLeftIndex, visitor);
        visitor(rootIndex);
        WalkThroughAllReferersByTargetCore(db, root->ByTargetRightIndex, visitor);
    }
}

int WalkThroughReferersByTargetCore(RawDB* db, link_index rootIndex, stoppable_visitor stoppableVisitor)
{
    if (rootIndex != null)
    {
        Link* root = GetLink(db, rootIndex);
        if (!WalkThroughReferersByTargetCore(db, root->ByTargetLeftIndex, stoppableVisitor)) return false;
        if (!stoppableVisitor(rootIndex)) return false;
        if (!WalkThroughReferersByTargetCore(db, root->ByTargetRightIndex, stoppableVisitor)) return false;
    }
    return true;
}

void public_calling_convention WalkThroughAllReferersByTarget(RawDB* db, link_index rootIndex, visitor visitor)
{
    if (rootIndex != null) WalkThroughAllReferersByTargetCore(db, GetLink(db, rootIndex)->ByTargetRootIndex, visitor);
}

signed_integer public_calling_convention WalkThroughReferersByTarget(RawDB* db, link_index rootIndex, stoppable_visitor stoppableVisitor)
{
    if (rootIndex != null) return WalkThroughReferersByTargetCore(db, GetLink(db, rootIndex)->ByTargetRootIndex, stoppableVisitor);
    else return true;
}

void AttachLink(RawDB* db, link_index linkIndex, uint64_t sourceIndex, uint64_t linkerIndex, uint64_t targetIndex)
{
    Link* link = GetLink(db, linkIndex);

    link->SourceIndex = sourceIndex;
    link->LinkerIndex = linkerIndex;
    link->TargetIndex = targetIndex;

    SubscribeAsRefererToSource(linkIndex, sourceIndex);
    SubscribeAsRefererToLinker(linkIndex, linkerIndex);
    SubscribeAsRefererToTarget(linkIndex, targetIndex);
}

void DetachLink(RawDB* db, link_index linkIndex)
{
    Link* link = GetLink(db, linkIndex);

    UnSubscribeFromSource(linkIndex, link->SourceIndex);
    UnSubscribeFromLinker(linkIndex, link->LinkerIndex);
    UnSubscribeFromTarget(linkIndex, link->TargetIndex);

    link->SourceIndex = null;
    link->LinkerIndex = null;
    link->TargetIndex = null;
}

void AttachLinkToUnusedMarker(RawDB* db, link_index linkIndex)
{
    SubscribeToListOfReferersBy(Linker, linkIndex, null);
}

void DetachLinkFromUnusedMarker(RawDB* db, link_index linkIndex)
{
    UnSubscribeFromListOfReferersBy(Linker, linkIndex, null);
}