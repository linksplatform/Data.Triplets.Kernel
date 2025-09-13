#ifndef __LINKS_LINKEDLIST_H__
#define __LINKS_LINKEDLIST_H__

// Linked-List

#define _GetNextSiblingRefererBy(byWhat, linkIndex) GetLink(linkIndex)->Concat3(By,byWhat,RightIndex)
#define _SetNextSiblingRefererBy(byWhat, linkIndex, newValue) GetLink(linkIndex)->Concat3(By,byWhat,RightIndex) = newValue

#define _GetPreviousSiblingRefererBy(byWhat, linkIndex) GetLink(linkIndex)->Concat3(By,byWhat,LeftIndex)
#define _SetPreviousSiblingRefererBy(byWhat, linkIndex, newValue) GetLink(linkIndex)->Concat3(By,byWhat,LeftIndex) = newValue

#define _GetFirstRefererBy(byWhat, linkIndex) GetLink(linkIndex)->Concat3(By,byWhat,RootIndex)
#define _SetFirstRefererBy(byWhat, linkIndex, newValue) GetLink(linkIndex)->Concat3(By,byWhat,RootIndex) = newValue

#define _IncrementNumberOfReferers(whichRererersBy, linkIndex) GetLink(linkIndex)->Concat3(By,whichRererersBy,Count)++
#define _DecrementNumberOfReferers(whichRererersBy, linkIndex) GetLink(linkIndex)->Concat3(By,whichRererersBy,Count)--

#define _GetNumberOfReferersBy(that, linkIndex) GetLink(linkIndex)->Concat3(By,that,Count)
#define _SetNumberOfReferersBy(that, linkIndex, newValue) GetLink(linkIndex)->Concat3(By,that,Count) = newValue

#define __GetNextSiblingRefererBySource(linkIndex) _GetNextSiblingRefererBy(Source, linkIndex)
#define __GetNextSiblingRefererByLinker(linkIndex) _GetNextSiblingRefererBy(Linker, linkIndex)
#define __GetNextSiblingRefererByTarget(linkIndex) _GetNextSiblingRefererBy(Target, linkIndex)

#define __SetNextSiblingRefererBySource(linkIndex, newValue) _SetNextSiblingRefererBy(Source, linkIndex, newValue)
#define __SetNextSiblingRefererByLinker(linkIndex, newValue) _SetNextSiblingRefererBy(Linker, linkIndex, newValue)
#define __SetNextSiblingRefererByTarget(linkIndex, newValue) _SetNextSiblingRefererBy(Target, linkIndex, newValue)

#define __GetPreviousSiblingRefererBySource(linkIndex) _GetPreviousSiblingRefererBy(Source, linkIndex)
#define __GetPreviousSiblingRefererByLinker(linkIndex) _GetPreviousSiblingRefererBy(Linker, linkIndex)
#define __GetPreviousSiblingRefererByTarget(linkIndex) _GetPreviousSiblingRefererBy(Target, linkIndex)

#define __SetPreviousSiblingRefererBySource(linkIndex, newValue) _SetPreviousSiblingRefererBy(Source, linkIndex, newValue)
#define __SetPreviousSiblingRefererByLinker(linkIndex, newValue) _SetPreviousSiblingRefererBy(Linker, linkIndex, newValue)
#define __SetPreviousSiblingRefererByTarget(linkIndex, newValue) _SetPreviousSiblingRefererBy(Target, linkIndex, newValue)

#define __GetNumberOfReferersBySource(linkIndex) _GetNumberOfReferersBy(Source, linkIndex)
#define __GetNumberOfReferersByLinker(linkIndex) _GetNumberOfReferersBy(Linker, linkIndex)
#define __GetNumberOfReferersByTarget(linkIndex) _GetNumberOfReferersBy(Target, linkIndex)

#define __SetNumberOfReferersBySource(linkIndex, newValue) _SetNumberOfReferersBy(Source, linkIndex, newValue)
#define __SetNumberOfReferersByLinker(linkIndex, newValue) _SetNumberOfReferersBy(Linker, linkIndex, newValue)
#define __SetNumberOfReferersByTarget(linkIndex, newValue) _SetNumberOfReferersBy(Target, linkIndex, newValue)

#define BeginWalkThroughReferersBySource(elementIndex, linkIndex) BeginWalkThroughLinksList(elementIndex, _GetFirstRefererBy(Source, linkIndex))
#define EndWalkThroughReferersBySource(elementIndex) EndWalkThroughLinksList(elementIndex, __GetNextSiblingRefererBySource)

#define BeginWalkThroughReferersByLinker(elementIndex, linkIndex) BeginWalkThroughLinksList(elementIndex, _GetFirstRefererBy(Linker, linkIndex))
#define EndWalkThroughReferersByLinker(elementIndex) EndWalkThroughLinksList(elementIndex, __GetNextSiblingRefererByLinker)

#define BeginWalkThroughReferersByTarget(elementIndex, linkIndex) BeginWalkThroughLinksList(elementIndex, _GetFirstRefererBy(Target, linkIndex))
#define EndWalkThroughReferersByTarget(elementIndex) EndWalkThroughLinksList(elementIndex, __GetNextSiblingRefererByTarget)

#define BeginWalkThroughLinksList(elementIndex, firstIndex) \
{                                                           \
    link_index firstElementIndex = firstIndex;              \
    if (firstElementIndex != null)                          \
    {                                                       \
        link_index elementIndex = firstElementIndex;        \
        do                                                  \
        {

#define EndWalkThroughLinksList(elementIndex, nextSelector) \
            elementIndex = nextSelector(elementIndex);      \
        }                                                   \
        while (elementIndex != firstElementIndex);          \
    }                                                       \
}

#define UnSubscribeFromListOfReferersBy(that, linkIndex, previousValue)                     \
{                                                                                           \
    link_index nextRefererIndex = _GetNextSiblingRefererBy(that,linkIndex);                 \
                                                                                            \
    if (nextRefererIndex != linkIndex)                                                      \
    {                                                                                       \
        link_index previousRefererIndex = _GetPreviousSiblingRefererBy(that,linkIndex);     \
                                                                                            \
        _SetPreviousSiblingRefererBy(that, of(nextRefererIndex), to(previousRefererIndex)); \
        _SetNextSiblingRefererBy(that, of(previousRefererIndex), to(nextRefererIndex));     \
                                                                                            \
        if (_GetFirstRefererBy(that, of(previousValue)) == linkIndex)                       \
            _SetFirstRefererBy(that, of(previousValue), to(nextRefererIndex));              \
    }                                                                                       \
    else if (_GetFirstRefererBy(that, of(previousValue)) == linkIndex)                      \
        _SetFirstRefererBy(that, of(previousValue), to(null));                              \
                                                                                            \
    _DecrementNumberOfReferers(that, of(previousValue));                                    \
    _SetNextSiblingRefererBy(that, of(linkIndex), to(null));                                \
    _SetPreviousSiblingRefererBy(that, of(linkIndex), to(null));                            \
}

#define SubscribeToListOfReferersBy(that, linkIndex, newValue)                                                   \
{                                                                                                                \
    link_index previousFirstRefererIndex = _GetFirstRefererBy(that, of(newValue));                               \
                                                                                                                 \
    if (previousFirstRefererIndex != null)                                                                       \
    {                                                                                                            \
        link_index previousLastRefererIndex = _GetPreviousSiblingRefererBy(that, of(previousFirstRefererIndex)); \
                                                                                                                 \
        _SetNextSiblingRefererBy(that, of(linkIndex), to(previousFirstRefererIndex));                            \
        _SetPreviousSiblingRefererBy(that, of(previousFirstRefererIndex), to(linkIndex));                        \
                                                                                                                 \
        _SetPreviousSiblingRefererBy(that, of(linkIndex), to(previousLastRefererIndex));                         \
        _SetNextSiblingRefererBy(that, of(previousLastRefererIndex), to(linkIndex));                             \
    }                                                                                                            \
    else                                                                                                         \
    {                                                                                                            \
        _SetNextSiblingRefererBy(that, of(linkIndex), to(linkIndex));                                            \
        _SetPreviousSiblingRefererBy(that, of(linkIndex), to(linkIndex));                                        \
    }                                                                                                            \
                                                                                                                 \
    _SetFirstRefererBy(that, of(newValue), to(linkIndex));                                                       \
                                                                                                                 \
    _IncrementNumberOfReferers(that, of(newValue));                                                              \
}

#endif