#include <gtest/gtest.h>
#include <PersistentMemoryManager.h>

namespace PlatformDataKernelTests
{

TEST(PersistentMemoryManagerTests, FileMappingTest)
{
    char* filename = (char*)"db.links";

    remove(filename);

    EXPECT_TRUE(succeeded(OpenLinks(filename)));

    EXPECT_TRUE(succeeded(CloseLinks()));

    remove(filename);
}

TEST(PersistentMemoryManagerTests, AllocateFreeLinkTest)
{
    char* filename = (char*)"db.links";

    remove(filename);

    EXPECT_TRUE(succeeded(OpenLinks(filename)));

    link_index link = AllocateLink();

    FreeLink(link);

    EXPECT_TRUE(succeeded(CloseLinks()));

    remove(filename);
}

TEST(PersistentMemoryManagerTests, AttachToUnusedLinkTest)
{
    char* filename = (char*)"db.links";

    remove(filename);

    EXPECT_TRUE(succeeded(OpenLinks(filename)));

    link_index link1 = AllocateLink();
    link_index link2 = AllocateLink();

    FreeLink(link1); // Creates "hole" and forces "Attach" to be executed

    EXPECT_TRUE(succeeded(CloseLinks()));

    remove(filename);
}

TEST(PersistentMemoryManagerTests, DetachToUnusedLinkTest)
{
    char* filename = (char*)"db.links";

    remove(filename);

    EXPECT_TRUE(succeeded(OpenLinks(filename)));

    link_index link1 = AllocateLink();
    link_index link2 = AllocateLink();

    FreeLink(link1); // Creates "hole" and forces "Attach" to be executed
    FreeLink(link2); // Removes both links, all "Attached" links forced to be "Detached" here

    EXPECT_TRUE(succeeded(CloseLinks()));

    remove(filename);
}

TEST(PersistentMemoryManagerTests, GetSetMappedLinkTest)
{
    char* filename = (char*)"db.links";

    remove(filename);

    EXPECT_TRUE(succeeded(OpenLinks(filename)));

    link_index mapped = GetMappedLink(0);

    SetMappedLink(0, mapped);

    EXPECT_TRUE(succeeded(CloseLinks()));

    remove(filename);
}
}
