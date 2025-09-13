#ifndef __LINKS_LINKLOWLEVEL_H__
#define __LINKS_LINKLOWLEVEL_H__

#define of(x) x
#define to(x) x
#define in(x) x

#define Concat(a, b) a##b
#define Concat3(a, b, c) a##b##c

#define _GetIndex(whatIndex, linkIndex) GetLink(linkIndex)->Concat(whatIndex,Index)
#define _GetLeftBy(byWhat, linkIndex) GetLink(linkIndex)->Concat3(By,byWhat,LeftIndex)
#define _SetLeftBy(byWhat, linkIndex, newValue) GetLink(linkIndex)->Concat3(By,byWhat,LeftIndex) = newValue
#define _GetRightBy(byWhat, linkIndex) GetLink(linkIndex)->Concat3(By,byWhat,RightIndex)
#define _SetRightBy(byWhat, linkIndex, newValue) GetLink(linkIndex)->Concat3(By,byWhat,RightIndex) = newValue
#define _GetCountBy(byWhat, linkIndex) GetLink(linkIndex)->Concat3(By,byWhat,Count)
#define _SetCountBy(byWhat, linkIndex, newValue) GetLink(linkIndex)->Concat3(By,byWhat,Count) = newValue

// Tree

#define __GetSourceIndex(linkIndex) _GetIndex(Source, linkIndex)
#define __GetLinkerIndex(linkIndex) _GetIndex(Linker, linkIndex)
#define __GetTargetIndex(linkIndex) _GetIndex(Target, linkIndex)

#define __GetLeftBySource(linkIndex) _GetLeftBy(Source, linkIndex)
#define __GetLeftByLinker(linkIndex) _GetLeftBy(Linker, linkIndex)
#define __GetLeftByTarget(linkIndex) _GetLeftBy(Target, linkIndex)

#define __GetRightBySource(linkIndex) _GetRightBy(Source, linkIndex)
#define __GetRightByLinker(linkIndex) _GetRightBy(Linker, linkIndex)
#define __GetRightByTarget(linkIndex) _GetRightBy(Target, linkIndex)

#define __GetCountBySource(linkIndex) _GetCountBy(Source, linkIndex)
#define __GetCountByLinker(linkIndex) _GetCountBy(Linker, linkIndex)
#define __GetCountByTarget(linkIndex) _GetCountBy(Target, linkIndex)

#define __SetLeftBySource(linkIndex, newValue) _SetLeftBy(Source, linkIndex, newValue)
#define __SetLeftByLinker(linkIndex, newValue) _SetLeftBy(Linker, linkIndex, newValue)
#define __SetLeftByTarget(linkIndex, newValue) _SetLeftBy(Target, linkIndex, newValue)

#define __SetRightBySource(linkIndex, newValue) _SetRightBy(Source, linkIndex, newValue)
#define __SetRightByLinker(linkIndex, newValue) _SetRightBy(Linker, linkIndex, newValue)
#define __SetRightByTarget(linkIndex, newValue) _SetRightBy(Target, linkIndex, newValue)

#define __SetCountBySource(linkIndex, newValue) _SetCountBy(Source, linkIndex, newValue)
#define __SetCountByLinker(linkIndex, newValue) _SetCountBy(Linker, linkIndex, newValue)
#define __SetCountByTarget(linkIndex, newValue) _SetCountBy(Target, linkIndex, newValue)

#include "LinkedList.h"

#define IsRefererLessThanOtherRefererBySourceCore(refererIndex, otherRefererLinkerIndex, otherRefererTargetIndex) (__GetLinkerIndex(refererIndex) < (otherRefererLinkerIndex) || (__GetLinkerIndex(refererIndex) == (otherRefererLinkerIndex) && __GetTargetIndex(refererIndex) < (otherRefererTargetIndex)))
#define IsRefererGreaterThanOtherRefererBySourceCore(refererIndex, otherRefererLinkerIndex, otherRefererTargetIndex) (__GetLinkerIndex(refererIndex) > (otherRefererLinkerIndex) || (__GetLinkerIndex(refererIndex) == (otherRefererLinkerIndex) && __GetTargetIndex(refererIndex) > (otherRefererTargetIndex)))

#define IsRefererLessThanOtherRefererBySource(refererIndex, otherRefererIndex) IsRefererLessThanOtherRefererBySourceCore(refererIndex, __GetLinkerIndex(otherRefererIndex), __GetTargetIndex(otherRefererIndex))
#define IsRefererGreaterThanOtherRefererBySource(refererIndex, otherRefererIndex) IsRefererGreaterThanOtherRefererBySourceCore(refererIndex, __GetLinkerIndex(otherRefererIndex), __GetTargetIndex(otherRefererIndex))

#define IsRefererLessThanOtherRefererByLinkerCore(refererIndex, otherRefererSourceIndex, otherRefererTargetIndex) (__GetSourceIndex(refererIndex) < (otherRefererSourceIndex) || (__GetSourceIndex(refererIndex) == (otherRefererSourceIndex) && __GetTargetIndex(refererIndex) < (otherRefererTargetIndex)))
#define IsRefererGreaterThanOtherRefererByLinkerCore(refererIndex, otherRefererSourceIndex, otherRefererTargetIndex) (__GetSourceIndex(refererIndex) > (otherRefererSourceIndex) || (__GetSourceIndex(refererIndex) == (otherRefererSourceIndex) && __GetTargetIndex(refererIndex) > (otherRefererTargetIndex)))

#define IsRefererLessThanOtherRefererByLinker(refererIndex, otherRefererIndex) IsRefererLessThanOtherRefererByLinkerCore(refererIndex, __GetSourceIndex(otherRefererIndex), __GetTargetIndex(otherRefererIndex))
#define IsRefererGreaterThanOtherRefererByLinker(refererIndex, otherRefererIndex) IsRefererGreaterThanOtherRefererByLinkerCore(refererIndex, __GetSourceIndex(otherRefererIndex), __GetTargetIndex(otherRefererIndex))

#define IsRefererLessThanOtherRefererByTargetCore(refererIndex, otherRefererLinkerIndex, otherRefererSourceIndex) (__GetLinkerIndex(refererIndex) < (otherRefererLinkerIndex) || (__GetLinkerIndex(refererIndex) == (otherRefererLinkerIndex) && __GetSourceIndex(refererIndex) < (otherRefererSourceIndex)))
#define IsRefererGreaterThanOtherRefererByTargetCore(refererIndex, otherRefererLinkerIndex, otherRefererSourceIndex) (__GetLinkerIndex(refererIndex) > (otherRefererLinkerIndex) || (__GetLinkerIndex(refererIndex) == (otherRefererLinkerIndex) && __GetSourceIndex(refererIndex) > (otherRefererSourceIndex)))

#define IsRefererLessThanOtherRefererByTarget(refererIndex, otherRefererIndex) IsRefererLessThanOtherRefererByTargetCore(refererIndex, __GetLinkerIndex(otherRefererIndex), __GetSourceIndex(otherRefererIndex))
#define IsRefererGreaterThanOtherRefererByTarget(refererIndex, otherRefererIndex) IsRefererGreaterThanOtherRefererByTargetCore(refererIndex, __GetLinkerIndex(otherRefererIndex), __GetSourceIndex(otherRefererIndex))

#define DefineReferersTreeLeftRotateMethod(referesToThat) \
    DefineTreeLeftRotateMethod( \
        Concat3(By,referesToThat,TreeLeftRotate), \
        link_index, \
        Concat(__GetLeftBy,referesToThat),Concat(__SetLeftBy,referesToThat), \
        Concat(__GetRightBy,referesToThat),Concat(__SetRightBy,referesToThat), \
        Concat(__GetCountBy,referesToThat),Concat(__SetCountBy,referesToThat))

#define DefineReferersTreeRightRotateMethod(referesToThat) \
    DefineTreeRightRotateMethod( \
        Concat3(By, referesToThat, TreeRightRotate), \
        link_index, \
        Concat(__GetLeftBy, referesToThat), Concat(__SetLeftBy, referesToThat), \
        Concat(__GetRightBy, referesToThat), Concat(__SetRightBy, referesToThat), \
        Concat(__GetCountBy, referesToThat), Concat(__SetCountBy, referesToThat))

#define DefineReferersTreeMaintainMethod(referesToThat) \
    DefineTreeMaintainMethods( \
        Concat3(By, referesToThat, TreeLeftMaintain), \
        Concat3(By, referesToThat, TreeRightMaintain), \
        link_index, \
        Concat3(By, referesToThat, TreeLeftRotate), \
        Concat3(By, referesToThat, TreeRightRotate), \
        Concat(__GetPreviousSiblingRefererBy, referesToThat), \
        Concat(__GetNextSiblingRefererBy, referesToThat), \
        Concat(__GetNumberOfReferersBy, referesToThat))

#define DefineReferersTreeInsertMethod(referesToThat) \
    DefineTreeInsertMethod(Concat3(By, referesToThat, TreeInsert), link_index, \
        Concat3(By, referesToThat, TreeLeftMaintain), \
        Concat3(By, referesToThat, TreeRightMaintain), \
        Concat(IsRefererLessThanOtherRefererBy, referesToThat), \
        Concat(__GetPreviousSiblingRefererBy, referesToThat), \
        Concat(__GetNextSiblingRefererBy, referesToThat), \
        Concat(__GetNumberOfReferersBy, referesToThat), \
        Concat(__SetNumberOfReferersBy, referesToThat))

#define DefineUnsafeDetachFromReferersTreeMethod(referesToThat) \
    DefineUnsafeDetachFromTreeMethod(Concat(UnsafeDetachFromTreeOfReferersBy, referesToThat), link_index, \
        Concat(IsRefererLessThanOtherRefererBy, referesToThat), \
        Concat(IsRefererGreaterThanOtherRefererBy, referesToThat), \
        Concat(__GetPreviousSiblingRefererBy, referesToThat), Concat(__SetPreviousSiblingRefererBy, referesToThat), \
        Concat(__GetNextSiblingRefererBy, referesToThat), Concat(__SetNextSiblingRefererBy, referesToThat), \
        Concat(__GetNumberOfReferersBy, referesToThat), Concat(__SetNumberOfReferersBy, referesToThat))

#define DefineAllReferersTreeMethods(referesToThat) \
    DefineReferersTreeLeftRotateMethod(referesToThat) \
    DefineReferersTreeRightRotateMethod(referesToThat) \
    DefineReferersTreeMaintainMethod(referesToThat) \
    DefineReferersTreeInsertMethod(referesToThat) \
    DefineUnsafeDetachFromReferersTreeMethod(referesToThat) \

#define DefineAllReferersBySourceTreeMethods() DefineAllReferersTreeMethods(Source)
#define DefineAllReferersByLinkerTreeMethods() DefineAllReferersTreeMethods(Linker)
#define DefineAllReferersByTargetTreeMethods() DefineAllReferersTreeMethods(Target)

#define BeginWalkThroughtReferersTree(byThat, element, root) BeginWalkThroughtTree(link_index, element, _GetFirstRefererBy(byThat, root), Concat(__GetLeftBy, byThat))
#define EndWalkThroughtReferersTree(byThat, element) EndWalkThroughtTree(element, Concat(__GetRightBy, byThat))

#define BeginWalkThroughtTreeOfReferersBySource(element, root) BeginWalkThroughtReferersTree(Source, element, root)
#define EndWalkThroughtTreeOfReferersBySource(element) EndWalkThroughtReferersTree(Source, element)

#define BeginWalkThroughtTreeOfReferersByLinker(element, root) BeginWalkThroughtReferersTree(Linker, element, root)
#define EndWalkThroughtTreeOfReferersByLinker(element) EndWalkThroughtReferersTree(Linker, element)

#define BeginWalkThroughtTreeOfReferersByTarget(element, root) BeginWalkThroughtReferersTree(Target, element, root)
#define EndWalkThroughtTreeOfReferersByTarget(element) EndWalkThroughtReferersTree(Target, element)

#define SubscribeToTreeOfReferersBy(that, linkIndex, newValue) Concat3(By, that, TreeInsert)(&_GetFirstRefererBy(that, of(newValue)), linkIndex)
#define UnSubscribeFromTreeOfReferersBy(that, linkIndex, newValue) Concat(UnsafeDetachFromTreeOfReferersBy, that)(&_GetFirstRefererBy(that, of(newValue)), linkIndex)

#define SubscribeAsRefererToSource(linkIndex, newValue) SubscribeToTreeOfReferersBy(Source, linkIndex, newValue)
#define SubscribeAsRefererToLinker(linkIndex, newValue) SubscribeToListOfReferersBy(Linker, linkIndex, newValue)
#define SubscribeAsRefererToTarget(linkIndex, newValue) SubscribeToTreeOfReferersBy(Target, linkIndex, newValue)

#define UnSubscribeFromSource(linkIndex, newValue) UnSubscribeFromTreeOfReferersBy(Source, linkIndex, newValue)
#define UnSubscribeFromLinker(linkIndex, newValue) UnSubscribeFromListOfReferersBy(Linker, linkIndex, newValue)
#define UnSubscribeFromTarget(linkIndex, newValue) UnSubscribeFromTreeOfReferersBy(Target, linkIndex, newValue)

#define GetNumberOfReferersInList(that, linkIndex) GetLink(linkIndex)->Concat3(By,that,Count)
#define GetNumberOfReferersInTree(that, linkIndex) _GetFirstRefererBy(that, of(linkIndex)) ? _GetNumberOfReferersBy(that, _GetFirstRefererBy(that, of(linkIndex))) : 0

#define GetNumberOfReferersBySource(linkIndex) GetNumberOfReferersInTree(Source, linkIndex)
#define GetNumberOfReferersByLinker(linkIndex) GetNumberOfReferersInList(Linker, linkIndex)
#define GetNumberOfReferersByTarget(linkIndex) GetNumberOfReferersInTree(Target, linkIndex)

#define DefineSearchInListOfReferersBySourceMethod()                                                                 \
link_index SearchRefererOfSource(link_index linkIndex, link_index refererTargetIndex, link_index refererLinkerIndex) \
{                                                                                                                    \
    BeginWalkThroughReferersByTarget(referer, in(linkIndex))                                                         \
        if (__GetTargetIndex(referer) == refererTargetIndex && __GetLinkerIndex(referer) == refererLinkerIndex)      \
            return referer;                                                                                          \
    EndWalkThroughReferersByTarget(referer);                                                                         \
    return null;                                                                                                     \
}

#define DefineSearchInListOfReferersByLinkerMethod()                                                                 \
link_index SearchRefererOfLinker(link_index linkIndex, link_index refererSourceIndex, link_index refererTargetIndex) \
{                                                                                                                    \
    BeginWalkThroughReferersByTarget(referer, in(linkIndex))                                                         \
        if (__GetSourceIndex(referer) == refererSourceIndex && __GetTargetIndex(referer) == refererTargetIndex)      \
            return referer;                                                                                          \
    EndWalkThroughReferersByTarget(referer);                                                                         \
    return null;                                                                                                     \
}

#define DefineSearchInListOfReferersByTargetMethod()                                                                 \
link_index SearchRefererOfTarget(link_index linkIndex, link_index refererSourceIndex, link_index refererLinkerIndex) \
{                                                                                                                    \
    BeginWalkThroughReferersByTarget(referer, in(linkIndex))                                                         \
        if (__GetSourceIndex(referer) == refererSourceIndex && __GetLinkerIndex(referer) == refererLinkerIndex)      \
            return referer;                                                                                          \
    EndWalkThroughReferersByTarget(referer);                                                                         \
    return null;                                                                                                     \
}

#define DefineSearchInTreeOfReferersBySourceMethod()                                                                 \
link_index SearchRefererOfSource(link_index linkIndex, link_index refererTargetIndex, link_index refererLinkerIndex) \
{                                                                                                                    \
    link_index currentNode = _GetFirstRefererBy(Source, of(linkIndex));                                              \
                                                                                                                     \
    while (currentNode)                                                                                              \
        if (IsRefererGreaterThanOtherRefererBySourceCore(currentNode, refererLinkerIndex, refererTargetIndex))       \
            currentNode = _GetPreviousSiblingRefererBy(Source, currentNode);                                         \
        else if (IsRefererLessThanOtherRefererBySourceCore(currentNode, refererLinkerIndex, refererTargetIndex))     \
            currentNode = _GetNextSiblingRefererBy(Source, currentNode);                                             \
        else                                                                                                         \
            return currentNode;                                                                                      \
                                                                                                                     \
    return null;                                                                                                     \
}

#define DefineSearchInTreeOfReferersByLinkerMethod()                                                                 \
link_index SearchRefererOfLinker(link_index linkIndex, link_index refererSourceIndex, link_index refererTargetIndex) \
{                                                                                                                    \
    link_index currentNode = _GetFirstRefererBy(Linker, of(linkIndex));                                              \
                                                                                                                     \
    while (currentNode)                                                                                              \
        if (IsRefererGreaterThanOtherRefererByLinkerCore(currentNode, refererSourceIndex, refererTargetIndex))       \
            currentNode = _GetPreviousSiblingRefererBy(Linker, currentNode);                                         \
        else if (IsRefererLessThanOtherRefererByLinkerCore(currentNode, refererSourceIndex, refererTargetIndex))     \
            currentNode = _GetNextSiblingRefererBy(Linker, currentNode);                                             \
        else                                                                                                         \
            return currentNode;                                                                                      \
                                                                                                                     \
    return null;                                                                                                     \
}

#define DefineSearchInTreeOfReferersByTargetMethod()                                                                 \
link_index SearchRefererOfTarget(link_index linkIndex, link_index refererSourceIndex, link_index refererLinkerIndex) \
{                                                                                                                    \
    link_index currentNode = _GetFirstRefererBy(Target, of(linkIndex));                                              \
                                                                                                                     \
    while (currentNode)                                                                                              \
        if (IsRefererGreaterThanOtherRefererByTargetCore(currentNode, refererLinkerIndex, refererSourceIndex))       \
            currentNode = _GetPreviousSiblingRefererBy(Target, currentNode);                                         \
        else if (IsRefererLessThanOtherRefererByTargetCore(currentNode, refererLinkerIndex, refererSourceIndex))     \
            currentNode = _GetNextSiblingRefererBy(Target, currentNode);                                             \
        else                                                                                                         \
            return currentNode;                                                                                      \
                                                                                                                     \
    return null;                                                                                                     \
}

#define DefineAllSearchMethods() \
    DefineSearchInTreeOfReferersBySourceMethod() \
    DefineSearchInListOfReferersByLinkerMethod() \
    DefineSearchInTreeOfReferersByTargetMethod()

#endif
