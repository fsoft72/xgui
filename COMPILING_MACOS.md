# Compiling XGUI for macOS

This guide explains how to compile XGUI for the latest Apple macOS versions (macOS 13+, including Ventura, Sonoma, Sequoia, and future releases).

## Prerequisites

### 1. Xcode Command Line Tools

Install the Xcode Command Line Tools, which include clang, clang++, and the macOS SDK:

```bash
xcode-select --install
```

Verify installation:
```bash
clang --version
```

### 2. Optional: Python Development (for Python bindings)

If you want to build Python bindings:
```bash
brew install python3
brew install swig
```

## Compilation Steps

### 1. Clean any previous builds

```bash
make -f Makefile.osx clean
```

### 2. Compile the test executable

```bash
make -f Makefile.osx test
```

This will:
- Compile all XGUI core objects (.cpp files)
- Compile all OpenStep/Cocoa implementation objects (.mm files)
- Link everything into an executable at `output/main`

### 3. Compile the shared library

```bash
make -f Makefile.osx lib
```

This will create `output/libxgui.dylib`, a dynamic library that can be linked into other applications.

### 4. Alternative: Create a Makefile symlink

For convenience, create a symlink to avoid typing `-f Makefile.osx` every time:

```bash
ln -s Makefile.osx Makefile
```

Then you can simply run:
```bash
make clean
make test
make lib
```

## What's New in the Updated Makefile

The Makefile.osx has been updated to support modern macOS versions with the following improvements:

1. **Modern Compiler**: Uses `clang` and `clang++` instead of `gcc/g++` (Apple's default compilers)

2. **SDK Targeting**: Targets macOS 13.0+ with `-mmacosx-version-min=13.0` for broad compatibility while supporting the latest macOS versions

3. **Memory Management**: Uses manual reference counting (MRC) to maintain compatibility with the existing OpenStep implementation code

4. **C++11 Standard**: Uses `-std=c++11` for modern C++ features

5. **Correct Framework Names**:
   - Fixed `Cocoa` framework (case-sensitive)
   - Added `Foundation` framework for additional macOS functionality

6. **macOS Dynamic Library**: Outputs `.dylib` files instead of Linux `.so` files, with proper `install_name` for runtime linking

## Troubleshooting

### SDK Version Issues

If you encounter SDK version errors, check your installed SDKs:
```bash
xcrun --show-sdk-path
xcrun --show-sdk-version
```

You can explicitly set the SDK path by modifying the `SDK_FLAGS` in Makefile.osx:
```makefile
SDK_FLAGS=-mmacosx-version-min=13.0 -isysroot $(shell xcrun --show-sdk-path)
```

### Framework Not Found

If Cocoa or Foundation frameworks are not found, ensure Xcode Command Line Tools are properly installed:
```bash
xcode-select -p
```

If not installed or misconfigured:
```bash
sudo xcode-select --reset
xcode-select --install
```

## Output Files

After successful compilation:

- **Test executable**: `output/main` - A test application demonstrating XGUI functionality
- **Shared library**: `output/libxgui.dylib` - The XGUI dynamic library for linking into other applications

## Next Steps

After compilation, you can:

1. Run the test application:
   ```bash
   ./output/main
   ```

2. Link the library into your own applications:
   ```bash
   clang++ your_app.cpp -o your_app -L./output -lxgui -Xlinker -rpath -Xlinker @executable_path
   ```

3. Install the library system-wide (optional):
   ```bash
   sudo cp output/libxgui.dylib /usr/local/lib/
   ```

## macOS Version Compatibility

The updated Makefile targets macOS 13.0 (Ventura) as the minimum deployment target, ensuring compatibility with:

- macOS 13 Ventura
- macOS 14 Sonoma
- macOS 15 Sequoia
- Future macOS releases

The code will run on any macOS version 13.0 or later.
