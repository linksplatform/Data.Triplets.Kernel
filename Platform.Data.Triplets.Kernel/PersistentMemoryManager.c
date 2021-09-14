// Менеджер памяти (RawDB* db, memory manager).

#include "Common.h"

#if defined(WINDOWS)
#include <windows.h>
#include <sys/stat.h>
#include <fcntl.h>
#elif defined(UNIX)
// for 64-bit files
#define _XOPEN_SOURCE 700
// for memset
#include <string.h>
#include <unistd.h>

// open(RawDB* db)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// errno
#include <errno.h>
// mmap(RawDB* db)...
#include <sys/mman.h>
#endif

#include "Link.h"
#include "PersistentMemoryManager.h"


typedef struct RawDB {
// Дескриптор файла базы данных и дескриптор объекта отображения (RawDB* db, map)
#if defined(WINDOWS)
HANDLE              storageFileHandle;
HANDLE              storageFileMappingHandle;
#elif defined(UNIX)
signed_integer      storageFileHandle;                  // для open(RawDB* db)
#endif
int64_t             storageFileSizeInBytes;             // Текущий размер файла.

void*               pointerToMappedRegion;              // указатель на начало региона памяти - результата mmap(RawDB* db)

// Константы, рассчитываемые при запуске приложения
int64_t             currentMemoryPageSizeInBytes;       // Размер страницы в операционной системе. Инициализируется в InitPersistentMemoryManager(RawDB* db);
int64_t             serviceBlockSizeInBytes;            // Размер сервисных данных, (две страницы). Инициализируется в InitPersistentMemoryManager(RawDB* db);
int64_t             baseLinksSizeInBytes;               // Размер массива базовых (привязанных) связей.
int64_t             baseBlockSizeInBytes;               // Базовый размер блока данных (шаг роста файла базы данных)
int64_t             storageFileMinSizeInBytes;          // Минимально возможный размер файла базы данных (Базовый размер блока (шага) + размер сервисного блока)


uint64_t*           pointerToDataSeal;                  // Указатель на уникальную печать, если она установлена, значит база данных открывается во второй или более раз.
uint64_t*           pointerToLinkIndexSize;             // Указатель на размер одного индекса связи.
uint64_t*           pointerToMappingLinksMaxSize;       // Указатель на максимальный размер массива базовых (привязанных) связей.
link_index*         pointerToPointerToMappingLinks;     // Указатель на начало массива базовых (привязанных) связей. Инициализируется в SetStorageFileMemoryMapping(RawDB* db).
link_index*         pointerToLinksMaxSize;              // Указатель на максимальный размер массива связей.
link_index*         pointerToLinksSize;                 // Указатель на текущий размер массива связей.
Link*               pointerToLinks;                     // Указатель на начало массива связей. Инициализируется в SetStorageFileMemoryMapping(RawDB* db).

Link*               pointerToUnusedMarker;              // Инициализируется в SetStorageFileMemoryMapping(RawDB* db)

} RawDB;

void PrintLinksDatabaseSize(RawDB* db)
{
#ifndef NDEBUG
    printf("Links database size: %" PRIu64 " links, %" PRIu64 " bytes for links. Service block size (RawDB* db, bytes): %" PRIu64 ".\n",
        (uint64_t)(*db->pointerToLinksSize),
        (uint64_t)(*db->pointerToLinksSize * sizeof(Link)),
        (uint64_t)db->serviceBlockSizeInBytes);
#endif
}

bool ExistsLink(RawDB* db, Link* link)
{
    return link && db->pointerToLinks != link && link->LinkerIndex; //link->SourceIndex && link->LinkerIndex && link->TargetIndex;
}

bool ExistsLinkIndex(RawDB* db, link_index linkIndex)
{
    return ExistsLink(db, GetLink(db, linkIndex));
}

bool IsNullLinkEmpty(RawDB* db)
{
    return !db->pointerToLinks->SourceIndex && !db->pointerToLinks->LinkerIndex && !db->pointerToLinks->TargetIndex;
}

Link* GetLink(RawDB* db, link_index linkIndex)
{
    return db->pointerToLinks + linkIndex;
}

link_index GetLinkIndex(RawDB* db, Link* link)
{
    return link - db->pointerToLinks;
}

unsigned_integer GetLinksCount(RawDB* db)
{
    return *db->pointerToLinksSize - 1;
}

unsigned_integer GetCurrentSystemPageSize()
{
#if defined(WINDOWS)

    //  typedef struct _SYSTEM_INFO {
    //      union {
    //          DWORD  dwOemId;
    //          struct {
    //              WORD wProcessorArchitecture;
    //              WORD wReserved;
    //          };
    //      };
    //      DWORD     dwPageSize;
    //      LPVOID    lpMinimumApplicationAddress;
    //      LPVOID    lpMaximumApplicationAddress;
    //      DWORD_PTR dwActiveProcessorMask;
    //      DWORD     dwNumberOfProcessors;
    //      DWORD     dwProcessorType;
    //      DWORD     dwAllocationGranularity;
    //      WORD      wProcessorLevel;
    //      WORD      wProcessorRevision;
    //  } SYSTEM_INFO;

    SYSTEM_INFO info; // см. http://msdn.microsoft.com/en-us/library/windows/desktop/ms724958%28v=vs.85%29.aspx
    GetSystemInfo(&info);
    return info.dwPageSize;

#elif defined(UNIX)

    long pageSize = sysconf(_SC_PAGESIZE);
    return pageSize;

#endif
}

void ResetStorageFile(RawDB* db)
{
#if defined(WINDOWS)
    db->storageFileHandle = INVALID_HANDLE_VALUE;
#elif defined(UNIX)
    db->storageFileHandle = -1;
#endif
    db->storageFileSizeInBytes = 0;
}

bool IsStorageFileOpened(RawDB* db)
{
#if defined(WINDOWS)
    return db->storageFileHandle != INVALID_HANDLE_VALUE;
#elif defined(UNIX)
    return db->storageFileHandle != -1;
#endif
}

signed_integer EnsureStorageFileOpened(RawDB* db)
{
    if (!IsStorageFileOpened(db))
    {
        ERROR_MESSAGE("Storage file is not open.");
        return ERROR_RESULT;
    }
    return SUCCESS_RESULT;
}

signed_integer EnsureStorageFileClosed(RawDB* db)
{
    if (IsStorageFileOpened(db))
    {
        ERROR_MESSAGE("Storage file is not closed.");
        return ERROR_RESULT;
    }
    return SUCCESS_RESULT;
}

signed_integer ResetStorageFileMapping(RawDB* db)
{
#if defined(WINDOWS)
    db->storageFileMappingHandle = INVALID_HANDLE_VALUE;
    db->pointerToMappedRegion = NULL;
#elif defined(UNIX)
    db->pointerToMappedRegion = MAP_FAILED;
#endif
    return (signed_integer)UINT64_MAX;
}

bool IsStorageFileMapped(RawDB* db)
{
#if defined(WINDOWS)
    return db->storageFileMappingHandle != INVALID_HANDLE_VALUE && db->pointerToMappedRegion != NULL;
#elif defined(UNIX)
    return db->pointerToMappedRegion != MAP_FAILED;
#endif
}

signed_integer EnsureStorageFileMapped(RawDB* db)
{
    if (!IsStorageFileMapped(db))
    {
        ERROR_MESSAGE("Storage file is not mapped.");
        return ERROR_RESULT;
    }
    return SUCCESS_RESULT;
}

signed_integer EnsureStorageFileUnmapped(RawDB* db)
{
    if (IsStorageFileMapped(db))
    {
        ERROR_MESSAGE("Storage file already mapped.");
        return ERROR_RESULT;
    }
    return SUCCESS_RESULT;
}

void InitPersistentMemoryManager(RawDB* db)
{
    db->currentMemoryPageSizeInBytes = GetCurrentSystemPageSize();
    db->serviceBlockSizeInBytes = db->currentMemoryPageSizeInBytes * 2;

    db->baseLinksSizeInBytes = db->serviceBlockSizeInBytes - sizeof(uint64_t) * 3 - sizeof(link_index) * 2;
    db->baseBlockSizeInBytes = db->currentMemoryPageSizeInBytes * 256 * 4 * sizeof(Link); // ~ 512 mb

    db->storageFileMinSizeInBytes = db->serviceBlockSizeInBytes + db->baseBlockSizeInBytes;

#ifndef NDEBUG
    printf("db->storageFileMinSizeInBytes = %" PRIu64 "\n", (uint64_t)db->storageFileMinSizeInBytes);
#endif

    ResetStorageFile(db);
    ResetStorageFileMapping(db);
}

signed_integer OpenStorageFile(RawDB* db, const char* filename)
{
    if (failed(EnsureStorageFileClosed(db)))
        return ERROR_RESULT;

    DEBUG_MESSAGE("Opening file...");

#if defined(WINDOWS)
    // см. MSDN "CreateFile function", http://msdn.microsoft.com/en-us/library/windows/desktop/aa363858%28v=vs.85%29.aspx
    db->storageFileHandle = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (db->storageFileHandle == INVALID_HANDLE_VALUE)
    {
        // см. MSDN "GetLastError function", http://msdn.microsoft.com/en-us/library/windows/desktop/ms679360%28v=vs.85%29.aspx
        ERROR_MESSAGE_WITH_CODE("Failed to open file.", GetLastError());
        return GetLastError();
    }
    // см. MSDN "GetFileSizeEx function", https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfilesizeex
    LARGE_INTEGER fileSize;
    if(!GetFileSizeEx(db->storageFileHandle, &fileSize))
    {
        ERROR_MESSAGE_WITH_CODE("Failed to get file size.", GetLastError());
        return GetLastError();
    }
    db->storageFileSizeInBytes = (int64_t)fileSize.QuadPart;
#elif  defined(UNIX)
    db->storageFileHandle = open(filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (db->storageFileHandle == -1)
    {
        ERROR_MESSAGE_WITH_CODE  ("Failed to open file.", errno);
        return errno;
    }

    struct stat statbuf;
    if (fstat(db->storageFileHandle, &statbuf) != 0)
    {
        ERROR_MESSAGE_WITH_CODE("Failed to get file size.", errno);
        return errno;
    }

    db->storageFileSizeInBytes = statbuf.st_size; // ? uint64_t = off_t
#endif

#ifndef NDEBUG
    printf("db->storageFileSizeInBytes = %" PRIu64 "\n", (uint64_t)db->storageFileSizeInBytes);

    printf("File %s opened.\n\n", filename);
#endif

    return SUCCESS_RESULT;
}

// Используется db->storageFileHandle и db->storageFileSizeInBytes для установки нового размера
signed_integer ResizeStorageFile(RawDB* db)
{
    if (succeeded(EnsureStorageFileOpened(db)))
    {
        if (succeeded(EnsureStorageFileUnmapped(db)))
        {
#if defined(WINDOWS)
            LARGE_INTEGER distanceToMoveFilePointer = { 0 };
            LARGE_INTEGER currentFilePointer = { 0 };
            if (!SetFilePointerEx(db->storageFileHandle, distanceToMoveFilePointer, &currentFilePointer, FILE_CURRENT))
            {
                ERROR_MESSAGE_WITH_CODE("Failed to get current file pointer.", GetLastError());
                return ERROR_RESULT;
            }

            distanceToMoveFilePointer.QuadPart = db->storageFileSizeInBytes - currentFilePointer.QuadPart;

            if (!SetFilePointerEx(db->storageFileHandle, distanceToMoveFilePointer, NULL, FILE_END))
            {
                ERROR_MESSAGE_WITH_CODE("Failed to set file pointer.", GetLastError());
                return ERROR_RESULT;
            }
            if (!SetEndOfFile(db->storageFileHandle))
            {
                ERROR_MESSAGE_WITH_CODE("Failed to set end of file.", GetLastError());
                return ERROR_RESULT;
            }
#elif defined(UNIX)
            // см. также под Linux, MAP_POPULATE
            // см. также mmap64() (size_t?)
            if (ftruncate(db->storageFileHandle, db->storageFileSizeInBytes) == -1)
            {
                ERROR_MESSAGE_WITH_CODE("Failed to resize file.", errno);
                return ERROR_RESULT;
            }
#endif
            return SUCCESS_RESULT;
        }
    }
    return ERROR_RESULT;
}

signed_integer SetStorageFileMemoryMapping(RawDB* db)
{
    if (failed(EnsureStorageFileOpened(db)))
        return ERROR_RESULT;

    if (failed(EnsureStorageFileUnmapped(db)))
        return ERROR_RESULT;

    DEBUG_MESSAGE("Setting memory mapping of storage file..");

    // по-крайней мере - минимальный блок для линков + сервисный блок
    if (db->storageFileSizeInBytes < db->storageFileMinSizeInBytes)
        db->storageFileSizeInBytes = db->storageFileMinSizeInBytes;

    // если блок линков выравнен неправильно (не кратен базовому размеру блока), выравниваем "вверх"
    if (((db->storageFileSizeInBytes - db->serviceBlockSizeInBytes) % db->baseBlockSizeInBytes) > 0)
        db->storageFileSizeInBytes = (((db->storageFileSizeInBytes - db->serviceBlockSizeInBytes) / db->baseBlockSizeInBytes) * db->baseBlockSizeInBytes) + db->storageFileMinSizeInBytes;

    ResizeStorageFile(db);

#if defined(WINDOWS)
    // см. MSDN "CreateFileMapping function", http://msdn.microsoft.com/en-us/library/windows/desktop/aa366537%28v=vs.85%29.aspx
    db->storageFileMappingHandle = CreateFileMapping(db->storageFileHandle, NULL, PAGE_READWRITE, 0, (DWORD)db->storageFileSizeInBytes, NULL);
    if (db->storageFileMappingHandle == INVALID_HANDLE_VALUE)
    {
        ERROR_MESSAGE_WITH_CODE("Mapping creation failed.", GetLastError());
        return ERROR_RESULT;
    }

    // аналог mmap(RawDB* db),
    // см. MSDN "MapViewOfFileEx function", http://msdn.microsoft.com/en-us/library/windows/desktop/aa366763%28v=vs.85%29.aspx
    // см. MSDN "MapViewOfFile function", http://msdn.microsoft.com/en-us/library/windows/desktop/aa366761%28v=vs.85%29.aspx
    // hFileMappingObject [in] A handle to a file mapping object. The CreateFileMapping and OpenFileMapping functions return this handle.
    db->pointerToMappedRegion = MapViewOfFileEx(db->storageFileMappingHandle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0, db->pointerToMappedRegion);
    if (db->pointerToMappedRegion == NULL)
    {
        ERROR_MESSAGE_WITH_CODE("Failed to set map view of file.", GetLastError());
        return ERROR_RESULT;
    }

#elif defined(UNIX)
    db->pointerToMappedRegion = mmap(NULL, db->storageFileSizeInBytes, PROT_READ | PROT_WRITE, MAP_SHARED, db->storageFileHandle, 0);

    if (db->pointerToMappedRegion == MAP_FAILED)
    {
        ERROR_MESSAGE_WITH_CODE("Failed to set map view of file.", errno);
        return ERROR_RESULT;
    }
#endif

    //       Storage File Structure
    //    ============================
    //   | Service Block              | *
    //   | DataSeal            !64bit | |
    //   | LinkIndexSize       !64bit | |
    //   | MappingLinksMaxSize !64bit | |
    //   | LinksMaxSize         64bit | |
    //   | LinksActualSize      64bit | | 2 * (RawDB* db, System Page Size)
    //   |              *             | |
    //   |   Base       |    Link     | |
    //   |  (Mapped)    |  indicies   | |
    //   |              *             | *
    //   | Links Block                | *
    //   |              *             | | Min after save: 1 * (RawDB* db, Link Size) // Needed for null link (RawDB* db, unused link marker)
    //   |   Actual     |    Link     | | Min on open: (db->BaseBlockSizeInBytes) // Grow step (default is 512 mb)
    //   |   Data       |  Structures | |
    //   |              |             | |
    //   |              *             | *
    //    ============================
    //    ! means it is always that size (RawDB* db, does not depend on link_index size)

    void* pointers[7] = {
        // Service Block
        (char*)db->pointerToMappedRegion + sizeof(uint64_t) * 0, // 0
        (char*)db->pointerToMappedRegion + sizeof(uint64_t) * 1, // 1
        (char*)db->pointerToMappedRegion + sizeof(uint64_t) * 2, // 2
        (char*)db->pointerToMappedRegion + sizeof(uint64_t) * 3 + sizeof(link_index) * 0, // 3
        (char*)db->pointerToMappedRegion + sizeof(uint64_t) * 3 + sizeof(link_index) * 1, // 4
        (char*)db->pointerToMappedRegion + sizeof(uint64_t) * 3 + sizeof(link_index) * 2, // 5

        // Links Block
        (char*)db->pointerToMappedRegion + db->serviceBlockSizeInBytes // 6
    };

    db->pointerToDataSeal = (uint64_t*)pointers[0];
    db->pointerToLinkIndexSize = (uint64_t*)pointers[1];
    db->pointerToMappingLinksMaxSize = (uint64_t*)pointers[2];
    db->pointerToLinksMaxSize = (link_index*)pointers[3];
    db->pointerToLinksSize = (link_index*)pointers[4];
    db->pointerToPointerToMappingLinks = (link_index*)pointers[5];

    db->pointerToLinks = (Link*)pointers[6];
    db->pointerToUnusedMarker = db->pointerToLinks;

#ifndef NDEBUG
    printf("DataSeal            = %" PRIu64 "\n", *db->pointerToDataSeal);
    printf("LinkIndexSize       = %" PRIu64 "\n", *db->pointerToLinkIndexSize);
    printf("MappingLinksMaxSize = %" PRIu64 "\n", *db->pointerToMappingLinksMaxSize);
    printf("LinksMaxSize        = %" PRIu64 "\n", (uint64_t)*db->pointerToLinksMaxSize);
    printf("LinksSize           = %" PRIu64 "\n", (uint64_t)*db->pointerToLinksSize);
    //printf("Un size           = %" PRIu64 "\n", (uint64_t)db->pointerToUnusedMarker->LinkerIndex);
#endif

    uint64_t expectedMappingLinksMaxSize = db->baseLinksSizeInBytes / sizeof(link_index);

    if (*db->pointerToDataSeal == LINKS_DATA_SEAL_64BIT)
    { // opening
        DEBUG_MESSAGE("Storage file opened.");

        if (*db->pointerToLinkIndexSize != sizeof(link_index))
        {
            ERROR_MESSAGE("Opening storage file with different link index size is not supported yet.");
            return ResetStorageFileMapping(db) & CloseStorageFile(db);
        }

        if (*db->pointerToMappingLinksMaxSize != expectedMappingLinksMaxSize)
        {
            ERROR_MESSAGE("Opening storage file with different system page size is not supported yet.");
            return ResetStorageFileMapping(db) & CloseStorageFile(db);
        }
        if (*db->pointerToLinksSize > *db->pointerToLinksMaxSize)
        {
            ERROR_MESSAGE("Saved links size counter is set to bigger value than maximum allowed size. Storage file is damaged.");
            return ResetStorageFileMapping(db) & CloseStorageFile(db);
        }

        *db->pointerToLinksMaxSize = (db->storageFileSizeInBytes - db->serviceBlockSizeInBytes) / sizeof(Link);

        // TODO: Varidate all mapped links are exist (otherwise reset them to 0) (RawDB* db, fast)
        // TODO: Varidate all freed link (RawDB* db, holes). (RawDB* db, slower)
        // TODO: Varidate all links. (RawDB* db, slowest)
    }
    else
    { // creation
        DEBUG_MESSAGE("Storage file created.");

        *db->pointerToLinkIndexSize = sizeof(link_index);
        *db->pointerToMappingLinksMaxSize = expectedMappingLinksMaxSize;
        *db->pointerToLinksMaxSize = (db->storageFileSizeInBytes - db->serviceBlockSizeInBytes) / sizeof(Link);
        *db->pointerToLinksSize = 1; // null element (RawDB* db, unused link marker) always exists

        // Only if mmap does not put zeros
        if (*db->pointerToPointerToMappingLinks)
            memset(db->pointerToPointerToMappingLinks, 0, db->baseLinksSizeInBytes);
        if (!IsNullLinkEmpty(db))
            memset(db->pointerToLinks, 0, sizeof(Link));
    }

    DEBUG_MESSAGE("Memory mapping of storage file is set.");

    PrintLinksDatabaseSize(db);

    return SUCCESS_RESULT;
}

signed_integer EnlargeStorageFile(RawDB* db)
{
    if (succeeded(EnsureStorageFileOpened(db)))
    {
        if (succeeded(EnsureStorageFileMapped(db)))
        {
            if (succeeded(ResetStorageFileMemoryMapping(db)))
            {
                if (db->storageFileSizeInBytes >= db->storageFileMinSizeInBytes)
                {
                    db->storageFileSizeInBytes += db->baseBlockSizeInBytes;
                }
                else
                {
                    ERROR_MESSAGE("File size is less than minimum allowed size.");
                    return ERROR_RESULT;
                }
                    
                if (succeeded(SetStorageFileMemoryMapping(db)))
                    return SUCCESS_RESULT;
            }
        }
    }

    return ERROR_RESULT;
}

signed_integer ShrinkStorageFile(RawDB* db)
{
    if (succeeded(EnsureStorageFileOpened(db)))
    {
        if (succeeded(EnsureStorageFileMapped(db)))
        {
            if (db->storageFileSizeInBytes > db->storageFileMinSizeInBytes)
            {
                link_index linksTableNewMaxSize = (db->storageFileSizeInBytes - db->serviceBlockSizeInBytes - db->baseBlockSizeInBytes) / sizeof(Link);

                if (*db->pointerToLinksSize < linksTableNewMaxSize)
                {
                    if (succeeded(ResetStorageFileMemoryMapping(db)))
                    {
                        db->storageFileSizeInBytes -= db->baseBlockSizeInBytes;

                        if (succeeded(SetStorageFileMemoryMapping(db)))
                            return SUCCESS_RESULT;
                    }
                }
            }
        }
    }

    return ERROR_RESULT;
}

signed_integer ResetStorageFileMemoryMapping(RawDB* db)
{
    if (succeeded(EnsureStorageFileOpened(db)))
    {
        if (succeeded(EnsureStorageFileMapped(db)))
        {
            DEBUG_MESSAGE("Resetting memory mapping of storage file...");

            PrintLinksDatabaseSize(db);

            if (*db->pointerToDataSeal != LINKS_DATA_SEAL_64BIT)
            {
                *db->pointerToDataSeal = LINKS_DATA_SEAL_64BIT; // Запечатываем файл
                DEBUG_MESSAGE("Storage file sealed.");
            }

            // Считаем реальный размер файла
            int64_t lastFileSizeInBytes = *db->pointerToDataSeal == LINKS_DATA_SEAL_64BIT ? (int64_t)(db->serviceBlockSizeInBytes + *db->pointerToLinksSize * sizeof(Link)) : db->storageFileSizeInBytes;

#if defined(WINDOWS)
            UnmapViewOfFile(db->pointerToMappedRegion);
            CloseHandle(db->storageFileMappingHandle);
#elif defined(UNIX)
            munmap(db->pointerToMappedRegion, db->storageFileSizeInBytes);
#endif

            ResetStorageFileMapping(db);

            // Обновляем текущий размер файла в соответствии с реальным (чтобы при закрытии файла сделать его размер минимальным).
            db->storageFileSizeInBytes = lastFileSizeInBytes;

            DEBUG_MESSAGE("Memory mapping of storage file is reset.");

            return SUCCESS_RESULT;
        }
    }

    return ERROR_RESULT;
}

signed_integer CloseStorageFile(RawDB* db)
{
    if (succeeded(EnsureStorageFileOpened(db)))
    {
        if (succeeded(EnsureStorageFileUnmapped(db)))
        {
            DEBUG_MESSAGE("Closing storage file...");

            // Перед закрытием файла обновляем его размер файла (это гарантирует его минимальный размер).
            ResizeStorageFile(db);

#if defined(WINDOWS)
            if (db->storageFileHandle == INVALID_HANDLE_VALUE)
            {
                // т.к. например STDIN_FILENO == 0 - для stdin (под Linux)
                // Убран принудительный выход, так как даже в случае неправильного дескриптора, его можно попытаться закрыть
                DEBUG_MESSAGE("Storage file is not open or already closed. Let's try to close it anyway.");
            } 

            CloseHandle(db->storageFileHandle);
#elif defined(UNIX)
            if (db->storageFileHandle == -1)
            {
                ERROR_MESSAGE("Storage file is not open or already closed.");
                return ERROR_RESULT;
            }
            close(db->storageFileHandle);
#endif

            ResetStorageFile(db);

            DEBUG_MESSAGE("Storage file closed.");

            return SUCCESS_RESULT;
        }
    }
    return ERROR_RESULT;
}

signed_integer OpenLinks(RawDB* db, const char* filename)
{
    InitPersistentMemoryManager(db);
    signed_integer result = OpenStorageFile(db, filename);
    if (!succeeded(result))
        return result;
    return SetStorageFileMemoryMapping(db);
}

signed_integer CloseLinks(RawDB* db)
{
    //signed_integer result = ResetStorageFileMemoryMapping(db);
    //if (!succeeded(result))
    //    return result;
    //return CloseStorageFile(db);
}

link_index AllocateFromUnusedLinks(RawDB* db)
{
    link_index unusedLinkIndex = db->pointerToUnusedMarker->ByLinkerRootIndex;
    DetachLinkFromUnusedMarker(db, unusedLinkIndex);
    return unusedLinkIndex;
}

link_index AllocateFromFreeLinks(RawDB* db)
{
    if (*db->pointerToLinksMaxSize == *db->pointerToLinksSize)
        EnlargeStorageFile(db);

    return (*db->pointerToLinksSize)++;
}

link_index AllocateLink(RawDB* db)
{
    if (db->pointerToUnusedMarker != null) {
        if (db->pointerToLinks->ByLinkerRootIndex != null) {
            return AllocateFromUnusedLinks(db);
        }
    }
    return AllocateFromFreeLinks(db);
}

void FreeLink(RawDB* db, link_index linkIndex)
{
    Link* link = GetLink(db, linkIndex);
    Link* lastUsedLink = db->pointerToLinks + *db->pointerToLinksSize - 1;

    if (link < lastUsedLink)
    {
        AttachLinkToUnusedMarker(db, linkIndex);
    }
    else if (link == lastUsedLink)
    {
        --*db->pointerToLinksSize;

        while ((--lastUsedLink)->LinkerIndex == null && db->pointerToLinks != lastUsedLink) // Не существует и не является 0-й связью
        {
            DetachLinkFromUnusedMarker(db, GetLinkIndex(db, lastUsedLink));
            --*db->pointerToLinksSize;
        }

        ShrinkStorageFile(db); // Размер будет уменьшен, только если допустимо
    }
}

void WalkThroughAllLinks(RawDB* db, visitor visitor)
{
    if (*db->pointerToLinksSize <= 1)
        return;

    Link* currentLink = db->pointerToLinks + 1;
    Link* lastLink = db->pointerToLinks + *db->pointerToLinksSize - 1;

    do {
        if (ExistsLink(db, currentLink)) visitor(GetLinkIndex(db, currentLink));
    } while (++currentLink <= lastLink);
}

signed_integer WalkThroughLinks(RawDB* db, stoppable_visitor stoppableVisitor)
{
    if (*db->pointerToLinksSize <= 1)
        return true;

    Link* currentLink = db->pointerToLinks + 1;
    Link* lastLink = db->pointerToLinks + *db->pointerToLinksSize - 1;

    do {
        if (ExistsLink(db, currentLink) && !stoppableVisitor(GetLinkIndex(db, currentLink))) return false;
    } while (++currentLink <= lastLink);

    return true;
}

link_index GetMappedLink(RawDB* db, signed_integer mappingIndex)
{
    if (mappingIndex >= 0 && mappingIndex < (signed_integer)*db->pointerToMappingLinksMaxSize)
        return db->pointerToPointerToMappingLinks[mappingIndex];
    else
        return null;
}

void SetMappedLink(RawDB* db, signed_integer mappingIndex, link_index linkIndex)
{
    if (mappingIndex >= 0 && mappingIndex < (signed_integer)*db->pointerToMappingLinksMaxSize)
        db->pointerToPointerToMappingLinks[mappingIndex] = linkIndex;
}


int nil(RawDB* db) {
    return 0;
}