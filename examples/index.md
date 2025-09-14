# Examples

## Basic Usage

### Creating Links

```c
#include "Link.h"

// Create a new link
Link link = CreateLink(source, target);

// Access link properties
LinkIndex source_id = GetSource(link);
LinkIndex target_id = GetTarget(link);
```

### Persistent Storage

```c
#include "PersistentMemoryManager.h"

// Initialize persistent storage
InitializePersistentMemoryManager("database.links");

// Links are automatically persisted
Link persistentLink = CreateLink(1, 2);

// Storage is automatically managed
ClosePersistentMemoryManager();
```

### Tree Operations

```c
#include "SizeBalancedTree.h"

// Tree operations are handled internally
// but can be customized for specific use cases

// Search operations
LinkIndex result = SearchInTree(root, key);

// Insertion maintains balance automatically
InsertIntoTree(&root, newNode);
```

## Advanced Usage

### Custom Memory Management

For applications requiring custom memory allocation strategies:

```c
// Set custom allocators before initialization
SetCustomAllocator(myMalloc, myFree);
InitializePersistentMemoryManager("custom.db");
```

### Debugging and Diagnostics

Enable debugging output during compilation:

```bash
# Add -DDEBUG to enable verbose output
make CFLAGS="-DDEBUG"
```

### Binary Database Inspection

View the binary database structure:

```bash
# Examine database file in hexadecimal
od -tx2 -w128 db.links | less -S
```

## Build Examples

### Linux Development

```bash
# Build library
make

# Run tests
./run.sh

# Clean build
make clean
```

### Windows with Visual Studio

```cmd
REM Open solution
start Platform.Data.Triplets.Kernel.sln

REM Or build from command line
msbuild Platform.Data.Triplets.Kernel.sln /p:Configuration=Release
```

### Cross-Platform Testing

The library includes comprehensive tests that run on all supported platforms:

- Unit tests for core functionality  
- Integration tests for persistence
- Performance benchmarks
- Memory leak detection

## Performance Considerations

### Optimization Tips

1. **Batch Operations**: Group related link operations together
2. **Memory Alignment**: Ensure proper data alignment for your platform
3. **Cache Locality**: Access related links sequentially when possible
4. **Persistence**: Configure persistence intervals based on your use case

### Benchmarking

Use the included benchmarking tools to measure performance:

```bash
# Run performance tests
make benchmark
./benchmark
```