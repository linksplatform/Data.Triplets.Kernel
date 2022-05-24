#ifndef __LINKS_LINK_H__
#define __LINKS_LINK_H__

// Высокоуровневая логика работы с связями.

#include "Common.h"

#define null    0LL
#define itself  0LL

typedef struct Link
{
    link_index          SourceIndex;            // Ссылка на начальную связь
    link_index          TargetIndex;            // Ссылка на конечную связь
    link_index          LinkerIndex;            // Ссылка на связь-связку (если разместить это поле после Source и Target, то вероятно это поможет проще конвертировать тройки в пары)
    signed_integer      Timestamp;
    /* Referers (RawDB* db, Index, Backlinks) */
    link_index          BySourceRootIndex;      // Ссылка на вершину дерева связей ссылающихся на эту связь в качестве начальной связи
    link_index          BySourceLeftIndex;      // Ссылка на левое поддерво связей ссылающихся на эту связь в качестве начальной связи
    link_index          BySourceRightIndex;     // Ссылка на правое поддерво связей ссылающихся на эту связь в качестве начальной связи
    unsigned_integer    BySourceCount;          // Количество связей ссылающихся на эту связь в качестве начальной связи (элементов в дереве)
    link_index          ByTargetRootIndex;      // Ссылка на вершину дерева связей ссылающихся на эту связь в качестве конечной связи
    link_index          ByTargetLeftIndex;      // Ссылка на левое поддерво связей ссылающихся на эту связь в качестве конечной связи
    link_index          ByTargetRightIndex;     // Ссылка на правое поддерво связей ссылающихся на эту связь в качестве конечной связи
    unsigned_integer    ByTargetCount;          // Количество связей ссылающихся на эту связь в качестве конечной связи (элементов в дереве)
    link_index          ByLinkerRootIndex;      // Ссылка на вершину дерева связей ссылающихся на эту связь в качестве связи связки
    link_index          ByLinkerLeftIndex;      // Ссылка на левое поддерво связей ссылающихся на эту связь в качестве связи связки
    link_index          ByLinkerRightIndex;     // Ссылка на правое поддерво связей ссылающихся на эту связь в качестве связи связки
    unsigned_integer    ByLinkerCount;          // Количество связей ссылающихся на эту связь в качестве связи связки (элементов в дереве)
} Link;


typedef signed_integer(*stoppable_visitor)(link_index); // Stoppable visitor callback (Останавливаемый обработчик для прохода по связям)
typedef void(*visitor)(link_index); // Visitor callback (Неостанавливаемый обработчик для прохода по связям)

typedef struct RawDB RawDB;

#if defined(__cplusplus)
extern "C" {
#endif

    PREFIX_DLL link_index GetSourceIndex(RawDB* db, link_index linkIndex);
    PREFIX_DLL link_index GetLinkerIndex(RawDB* db, link_index linkIndex);
    PREFIX_DLL link_index GetTargetIndex(RawDB* db, link_index linkIndex);
    PREFIX_DLL signed_integer GetTime(RawDB* db, link_index linkIndex);

    PREFIX_DLL link_index CreateLink(RawDB* db, link_index sourceIndex, link_index linkerIndex, link_index targetIndex);

    PREFIX_DLL link_index SearchLink(RawDB* db, link_index sourceIndex, link_index linkerIndex, link_index targetIndex);

    PREFIX_DLL link_index ReplaceLink(RawDB* db, link_index linkIndex, link_index replacementIndex);
    PREFIX_DLL link_index UpdateLink(RawDB* db, link_index linkIndex, link_index sourceIndex, link_index linkerIndex, link_index targetIndex);

    PREFIX_DLL void DeleteLink(RawDB* db, link_index linkIndex);

    PREFIX_DLL link_index GetFirstRefererBySourceIndex(RawDB* db, link_index linkIndex);
    PREFIX_DLL link_index GetFirstRefererByLinkerIndex(RawDB* db, link_index linkIndex);
    PREFIX_DLL link_index GetFirstRefererByTargetIndex(RawDB* db, link_index linkIndex);

    PREFIX_DLL unsigned_integer GetLinkNumberOfReferersBySource(RawDB* db, link_index linkIndex);
    PREFIX_DLL unsigned_integer GetLinkNumberOfReferersByLinker(RawDB* db, link_index linkIndex);
    PREFIX_DLL unsigned_integer GetLinkNumberOfReferersByTarget(RawDB* db, link_index linkIndex);

    PREFIX_DLL void WalkThroughAllReferersBySource(RawDB* db, link_index rootIndex, visitor);
    // PREFIX_DLL void WalkThroughAllReferersBySource1(link_index rootIndex, visitor);
    PREFIX_DLL signed_integer WalkThroughReferersBySource(RawDB* db, link_index rootIndex, stoppable_visitor stoppableVisitor);

    PREFIX_DLL void WalkThroughAllReferersByLinker(RawDB* db, link_index rootIndex, visitor);
    PREFIX_DLL signed_integer WalkThroughReferersByLinker(RawDB* db, link_index rootIndex, stoppable_visitor stoppableVisitor);

    PREFIX_DLL void WalkThroughAllReferersByTarget(RawDB* db, link_index rootIndex, visitor);
    PREFIX_DLL signed_integer WalkThroughReferersByTarget(RawDB* db, link_index rootIndex, stoppable_visitor stoppableVisitor);

    /* "Unused marker" help mark links that was deleted, but still can be reused */

    PREFIX_DLL void AttachLinkToUnusedMarker(RawDB* db, link_index linkIndex);
    PREFIX_DLL void DetachLinkFromUnusedMarker(RawDB* db, link_index linkIndex);

    PREFIX_DLL void AttachLink(RawDB* db, link_index linkIndex, uint64_t sourceIndex, uint64_t linkerIndex, uint64_t targetIndex);
    PREFIX_DLL void DetachLink(RawDB* db, link_index linkIndex);

#if defined(__cplusplus)
}
#endif

#endif
