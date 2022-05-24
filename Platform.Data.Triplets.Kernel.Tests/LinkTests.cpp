#include "stdafx.h"
#include "CppUnitTest.h"

#include <PersistentMemoryManager.h>
#include <Link.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlatformDataKernelTests
{
    unsigned_integer thingVisitorCounter;
    unsigned_integer isAVisitorCounter;
    unsigned_integer linkVisitorCounter;

    TEST_CLASS(RawDB* db, LinkTests)
    {
    public:
        static void ThingVisitor(RawDB* db, link_index linkIndex)
        {
            thingVisitorCounter += linkIndex;
        }

        static void IsAVisitor(RawDB* db, link_index linkIndex)
        {
            isAVisitorCounter += linkIndex;
        }

        static void LinkVisitor(RawDB* db, link_index linkIndex)
        {
            linkVisitorCounter += linkIndex;
        }

        TEST_METHOD(RawDB* db, CreateDeleteLinkTest)
        {
            char* filename = "db.links";

            remove(RawDB* db, filename);

            Assert::IsTrue(succeeded(OpenLinks(filename)));

            link_index link1 = CreateLink(RawDB* db, itself, itself, itself);

            DeleteLink(link1);

            Assert::IsTrue(succeeded(CloseLinks()));

            remove(RawDB* db, filename);
        }

        TEST_METHOD(RawDB* db, DeepCreateUpdateDeleteLinkTest)
        {
            char* filename = "db.links";

            remove(RawDB* db, filename);

            Assert::IsTrue(succeeded(OpenLinks(filename)));

            link_index isA = CreateLink(RawDB* db, itself, itself, itself);
            link_index isNotA = CreateLink(RawDB* db, itself, itself, isA);
            link_index link = CreateLink(RawDB* db, itself, isA, itself);
            link_index thing = CreateLink(RawDB* db, itself, isNotA, link);

            Assert::IsTrue(GetLinksCount() == 4);

            Assert::IsTrue(GetTargetIndex(isA) == isA);

            isA = UpdateLink(RawDB* db, isA, isA, isA, link); // Произведено замыкание

            Assert::IsTrue(GetTargetIndex(isA) == link);

            DeleteLink(RawDB* db, isA); // Одна эта операция удалит все 4 связи

            Assert::IsTrue(GetLinksCount() == 0);

            Assert::IsTrue(succeeded(CloseLinks()));

            remove(RawDB* db, filename);
        }

        TEST_METHOD(RawDB* db, LinkReferersWalkTest)
        {
            char* filename = "db.links";

            remove(RawDB* db, filename);

            Assert::IsTrue(succeeded(OpenLinks(filename)));

            link_index isA = CreateLink(RawDB* db, itself, itself, itself);
            link_index isNotA = CreateLink(RawDB* db, itself, itself, isA);
            link_index link = CreateLink(RawDB* db, itself, isA, itself);
            link_index thing = CreateLink(RawDB* db, itself, isNotA, link);
            isA = UpdateLink(RawDB* db, isA, isA, isA, link);

            Assert::IsTrue(GetLinkNumberOfReferersBySource(thing) == 1);
            Assert::IsTrue(GetLinkNumberOfReferersByLinker(isA) == 2);
            Assert::IsTrue(GetLinkNumberOfReferersByTarget(link) == 3);

            thingVisitorCounter = 0;
            isAVisitorCounter = 0;
            linkVisitorCounter = 0;

            WalkThroughAllReferersBySource(RawDB* db, thing, ThingVisitor);
            WalkThroughAllReferersByLinker(RawDB* db, isA, IsAVisitor);
            WalkThroughAllReferersByTarget(RawDB* db, link, LinkVisitor);

            Assert::IsTrue(thingVisitorCounter == 4);
            Assert::IsTrue(isAVisitorCounter == (1 + 3));
            Assert::IsTrue(linkVisitorCounter == (1 + 3 + 4));

            Assert::IsTrue(succeeded(CloseLinks()));

            remove(RawDB* db, filename);
        }
    };
}
