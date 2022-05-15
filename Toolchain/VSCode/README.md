# IDE: VS Code 
## VS Code Extensions 
- C/C++
- CMake
- CMakeTools
- Cortex-Debug (for SWD debugging)

# VSARM Utilities
Total size: approximately 4.3 GB.

## Generator: MinGW
32-bit MinGW-W64 files are adequate, e.g. [i686-win32-sjlj](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/8.1.0/threads-posix/sjlj/i686-8.1.0-release-posix-sjlj-rt_v6-rev0.7z).

Set the directory of the mingw32-make.exe to be included in the CMake build and configure environments' `PATH` vars and `cmake.mingwSearchDirs`.  
Additionally, set `cmake.generator` to be "MinGW Makefiles".

## Builder: CMake
The 64-bit Windows CMake binary will do, e.g. [cmake-3.23.1-windows-x86_64.zip](https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1-windows-x86_64.zip). 

Set `cmake.cmakePath` to the location of the cmake.exe.

Add the arm-gnu toolkit location to the `cmake-tools-kits.json` file,  [like so](/Toolchain/VSCode/cmake-tools-kits.json).

## Compiler: Arm-GNU
Only the simplest version is needed, e.g., [gcc-arm-11.2-2022.02-mingw-w64-i686-arm-none-eabi.zip](https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-mingw-w64-i686-arm-none-eabi.zip).

Set `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` to locate this, in both the CMake Configure and Build environments (or as an env var).

Note that arm-none-eabi-gcc.exe cannot be run from/on networked files without elevated networked permissions. This may prevent using network drives as the root directory of your VSARM development in some locations, e.g. school or work computers.

## SDK: pico-sdk
The pico-sdk can be [cloned from github](https://github.com/raspberrypi/pico-sdk).

Set `PICO_SDK_PATH` in both the CMake configure environment (or as an env var).  
Additionally, set `PICO_TOOLCHAIN_PATH` in the CMake Configure environment  (or as an env var) to the Arm-GNU-gcc executable.

Don't forget to run `git submodule update --init` to clone the tinyusb dependency.

## Kernel: FreeRTOS-Kernel
The FreeRTOS kernel can be [cloned from github](https://github.com/FreeRTOS/FreeRTOS-Kernel/).

Set `FREERTOS_KERNEL_PATH` in the CMake Configure environment (or as an env var) to the root folder of the kernel repo.