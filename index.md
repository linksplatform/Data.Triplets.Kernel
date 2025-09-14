# Data.Triplets.Kernel Documentation

Welcome to the Data.Triplets.Kernel documentation site.

## Overview

Data.Triplets.Kernel is a high-performance library for working with triplet data structures, providing efficient storage and manipulation of link-based data.

## Features

- **High Performance**: Optimized C implementation for maximum speed
- **Persistent Storage**: Efficient on-disk storage management
- **Cross Platform**: Supports Linux, Windows, and macOS
- **Memory Efficient**: Advanced memory management with size-balanced trees

## Quick Start

### Installation

The library is available as a NuGet package:

```
Install-Package Platform.Data.Triplets.Kernel
```

### Building from Source

#### Linux
```bash
make
./run.sh
```

#### Windows (Visual Studio)
1. Open `Platform.Data.Triplets.Kernel.sln`
2. Press `CTRL+SHIFT+B` to build

#### Windows (MinGW)
```cmd
mingw32-make
test
```

## API Reference

### Core Components

- **Link**: Core data structure representing triplet relationships
- **PersistentMemoryManager**: Handles persistent storage operations
- **SizeBalancedTree**: Efficient tree structure for data organization
- **Timestamp**: Unique timestamp generation utilities

## Resources

- [Source Code](https://github.com/linksplatform/Data.Triplets.Kernel)
- [NuGet Package](https://www.nuget.org/packages/Platform.Data.Triplets.Kernel)
- [Issues](https://github.com/linksplatform/Data.Triplets.Kernel/issues)

## License

This project is unlicensed - see the [LICENSE](https://github.com/linksplatform/Data.Triplets.Kernel/blob/main/LICENSE) file for details.