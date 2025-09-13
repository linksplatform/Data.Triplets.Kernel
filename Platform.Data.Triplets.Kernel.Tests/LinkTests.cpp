#include <gtest/gtest.h>
#include <PersistentMemoryManager.h>
#include <Link.h>

namespace PlatformDataKernelTests
{
    unsigned_integer thingVisitorCounter;
    unsigned_integer isAVisitorCounter;
    unsigned_integer linkVisitorCounter;
    static void ThingVisitor(link_index linkIndex)
    {
        thingVisitorCounter += linkIndex;
    }

    static void IsAVisitor(link_index linkIndex)
    {
        isAVisitorCounter += linkIndex;
    }

    static void LinkVisitor(link_index linkIndex)
    {
        linkVisitorCounter += linkIndex;
    }

TEST(LinkTests, CreateDeleteLinkTest)
{
    char* filename = (char*)"db.links";

    remove(filename);

    EXPECT_TRUE(succeeded(OpenLinks(filename)));

    link_index link1 = CreateLink(itself, itself, itself);

    DeleteLink(link1);

    EXPECT_TRUE(succeeded(CloseLinks()));

    remove(filename);
}

TEST(LinkTests, DeepCreateUpdateDeleteLinkTest)
{
    char* filename = (char*)"db.links";

    remove(filename);

    EXPECT_TRUE(succeeded(OpenLinks(filename)));

    link_index isA = CreateLink(itself, itself, itself);
    link_index isNotA = CreateLink(itself, itself, isA);
    link_index link = CreateLink(itself, isA, itself);
    link_index thing = CreateLink(itself, isNotA, link);

    EXPECT_TRUE(GetLinksCount() == 4);

    EXPECT_TRUE(GetTargetIndex(isA) == isA);

    isA = UpdateLink(isA, isA, isA, link); // Произведено замыкание

    EXPECT_TRUE(GetTargetIndex(isA) == link);

    DeleteLink(isA); // Одна эта операция удалит все 4 связи

    EXPECT_TRUE(GetLinksCount() == 0);

    EXPECT_TRUE(succeeded(CloseLinks()));

    remove(filename);
}

TEST(LinkTests, LinkReferersWalkTest)
{
    char* filename = (char*)"db.links";

    remove(filename);

    EXPECT_TRUE(succeeded(OpenLinks(filename)));

    link_index isA = CreateLink(itself, itself, itself);
    link_index isNotA = CreateLink(itself, itself, isA);
    link_index link = CreateLink(itself, isA, itself);
    link_index thing = CreateLink(itself, isNotA, link);
    isA = UpdateLink(isA, isA, isA, link);

    EXPECT_TRUE(GetLinkNumberOfReferersBySource(thing) == 1);
    EXPECT_TRUE(GetLinkNumberOfReferersByLinker(isA) == 2);
    EXPECT_TRUE(GetLinkNumberOfReferersByTarget(link) == 3);

    thingVisitorCounter = 0;
    isAVisitorCounter = 0;
    linkVisitorCounter = 0;

    WalkThroughAllReferersBySource(thing, ThingVisitor);
    WalkThroughAllReferersByLinker(isA, IsAVisitor);
    WalkThroughAllReferersByTarget(link, LinkVisitor);

    EXPECT_TRUE(thingVisitorCounter == 4);
    EXPECT_TRUE(isAVisitorCounter == (1 + 3));
    EXPECT_TRUE(linkVisitorCounter == (1 + 3 + 4));

    EXPECT_TRUE(succeeded(CloseLinks()));

    remove(filename);
}
}
