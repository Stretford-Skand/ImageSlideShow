# Image Slideshow

A desktop slideshow application built with **C++17** and **Qt** (supports Qt 5.15+ and Qt 6.x).

Select a folder of images and the app streams them at a configurable interval in a dedicated worker thread, with optional horizontal/vertical mirroring.

## Features

- Browse and select any folder containing images (TIFF, BMP, JPG/JPEG)
- Configurable slide interval (1 – ∞ seconds via spin box)
- Play / Pause / Resume controls with icon feedback
- Horizontal, Vertical, or Both-axis image mirroring
- Optional **Reload** button to rescan the folder without restarting (compile-time option)
- All events logged to `log.txt` alongside the executable
- Graceful recovery: if an image disappears during a slideshow the folder is automatically rescanned

## Build

### Prerequisites

- CMake 3.14+
- Qt 5.15+ **or** Qt 6.x (Core + Widgets modules)
- A C++17 compiler (MSVC 2019+, GCC 9+, Clang 10+)

### Configure and build

```bash
# Qt found automatically if it is on PATH / CMAKE_PREFIX_PATH
cmake -B build
cmake --build build

# Or point at a specific Qt installation
cmake -B build -DQT_ROOT_DIR="C:/Qt/6.7.0/msvc2019_64"
cmake --build build

# Force Qt 5 instead of Qt 6
cmake -B build -DQT_DEFAULT_MAJOR_VERSION=5
cmake --build build

# Build without the Reload button
cmake -B build -DADD_RELOAD_BUTTON=OFF
cmake --build build
```

The binary is placed in `bin/`.

## Code quality

This project ships with [clang-format](.clang-format) (Allman style, C++17) and [clang-tidy](.clang-tidy) configs.

```bash
# Format all source files in-place
clang-format -i src/*.cpp src/*.h

# Run static analysis manually
clang-tidy src/*.cpp -- -std=c++17 -I src

# Or build with analysis baked in (requires clang-tidy on PATH)
cmake -B build -DENABLE_CLANG_TIDY=ON
cmake --build build
```

## Supported image formats

`*.tiff`, `*.bmp`, `*.jpg`, `*.jpeg`

## License

This project is provided as a portfolio sample.
