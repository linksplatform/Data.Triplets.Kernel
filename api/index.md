# API Reference

## Core Data Structures

### Link
The fundamental triplet data structure representing relationships between entities.

#### Key Functions
- Link creation and manipulation
- Relationship traversal
- Memory-efficient storage

### PersistentMemoryManager
Manages persistent storage of link data with efficient disk operations.

#### Features
- Automatic persistence to disk
- Memory-mapped file access
- Crash recovery support

### SizeBalancedTree
Self-balancing tree structure optimized for link storage and retrieval.

#### Characteristics
- O(log n) search, insert, delete operations
- Memory-efficient node structure
- Automatic rebalancing

### Timestamp
Provides guaranteed unique timestamp generation for link versioning.

#### Usage
- Conflict-free timestamp generation
- High-resolution timing
- Thread-safe operations

## Build Configuration

### Preprocessor Definitions
- `DEBUG`: Enable debug output
- Platform-specific macros for cross-platform compatibility

### Compilation Targets
- **Linux**: `make` produces shared library
- **Windows**: Visual Studio or MinGW builds
- **macOS**: Compatible with standard build tools

## Memory Layout

The library uses efficient memory layouts optimized for:
- Cache performance
- Memory alignment
- Minimal fragmentation

## Error Handling

Functions return standard error codes:
- 0: Success
- Non-zero: Error conditions with specific meanings

## Thread Safety

The library provides thread-safe operations where appropriate, with specific documentation for each function's concurrency guarantees.