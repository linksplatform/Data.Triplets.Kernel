[![NuGet Version and Downloads count](https://buildstats.info/nuget/Platform.Data.Triplets.Kernel)](https://www.nuget.org/packages/Platform.Data.Triplets.Kernel)
[![Build Status](https://travis-ci.com/linksplatform/Data.Triplets.Kernel.svg?branch=master)](https://travis-ci.com/linksplatform/Data.Triplets.Kernel)

# Data.Triplets.Kernel

## NuGet Package

[Platform.Data.Triplets.Kernel](https://www.nuget.org/packages/Platform.Data.Triplets.Kernel)

## Useful links to understand code
* [GNU C Macros](https://gcc.gnu.org/onlinedocs/cpp/Macros.html#Macros)
* [C/C++ Preprocessor Reference](https://docs.microsoft.com/en-us/cpp/preprocessor/c-cpp-preprocessor-reference?view=vs-2019)

## Compile and Run

### On Linux

Build library and test for it:
```
$ make
```

The Makefile configured to build the library as `Platform.Data.Triplets.Kernel` on any platform right now. But at autodeploy these libraries renamed into `Platform_Data_Triplets_Kernel.dll` (for Windows), `libPlatform_Data_Triplets_Kernel.so` (Linux) and `libPlatform_Data_Triplets_Kernel.dylib` (macOS). Latest version of binaries can be found at [binaries](https://github.com/linksplatform/Data.Triplets.Kernel/tree/binaries) branch.

Run test:
```
$ ./run.sh
```

To enable debug output put `-DDEBUG` option into makefile.

Compiled library will be available at `Platform.Data.Triplets.Kernel` folder as `Platform_Data_Triplets_Kernel` file.

To view resulting database file in binary:
```
$ od -tx2 -w128 db.links | less -S
```

### On Windows

To build the code on Windows the compiler is required:

1. [Visual Studio](https://visualstudio.microsoft.com/vs)
2. [MinGW](http://www.mingw.org)

#### Using Visual Studio

Open `Platform.Data.Triplets.Kernel.sln` using Visual Studio (can be found in root folder of repository)

Press `CTRL+SHIFT+B` or `F6` or use menu item (`Build Solution` or `Build Platform.Data.Kernel`) from `Build` menu.

Compiled library will be available at `Debug`/`Release` folder of in root folder of repository as `Platform.Data.Triplets.Kernel.dll` file.

To Run tests in Visual Studio use `Test Explorer`. Actual test are located at `Platform.Data.Triplets.Kernel.Tests` project.

#### Using MinGW

Run `cmd` with administrator rights.

Change directory `cd` to `Platform\Platform.Data.Kernel` folder.

Build library and test for it:
```
$ mingw32-make
```

Run test:
```
$ test
```

To enable debug output put `-DDEBUG` option into makefile.

Compiled library will be available at `Platform.Data.Triplets.Kernel` folder as `Platform.Data.Triplets.Kernel` file.

You can use any HEX Viewer/Editor to check for `db.links` structure after the `test` was run.
