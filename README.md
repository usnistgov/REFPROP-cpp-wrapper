# REFPROP wrapper for C/C++
This repository contains a CMake-based build system for the NIST REFPROP shared library (e.g., DLL) and a C/C++ wrapper for calling it. Wrappers are available in other languages at https://github.com/usnistgov/REFPROP-wrappers/.
 
Installation
------------
**NOTE**: for Linux and macOS, you must compile REFPROP yourself, see: [REFPROP-cmake](https://github.com/usnistgov/REFPROP-cmake)

1. Follow the conventional installation instructions for REFPROP.
2. For Linux, compile REFPROP via [REFPROP-cmake](https://github.com/usnistgov/REFPROP-cmake) and copy the resulting `librefprop.so` library file to the REFPROP installation directory.
3. Ensure that the system environment variable `RPprefix` is set to the location of the REFPROP installation (e.g., `C:\Program Files (x86)\REFPROP`).
4. Install CMake, version 3.22 or newer.
   1. Windows: https://cmake.org/download/
   2. Linux: `sudo apt install cmake`
5. Install Git, version 2.0 or newer.
   1. Windows: https://git-scm.com/downloads
   2. Linux: `sudo apt install git`
6. Install compilers and build tools.
   1. Windows:  Install the Microsoft Visual C++ compiler (MSVC).
      1. **Note:** This compiler is included with the Visual Studio IDE and this step can be skipped if that is already installed.)
      2. Go to the Visual Studio download page (https://visualstudio.microsoft.com/downloads/).
      3. Under "Tools for Visual Studio", download "Build Tools for Visual Studio 20XX".
      4. Launch the installer and select the "C++ build tools" workload, ensuring that the MSVC compiler, Windows SDK and C++ Standard Library are included.
      5. Complete the installation.
   2. Linux: `sudo apt install build-essential`
7. Either clone this repository, download as a ZIP file and extract the contents or download just the files in the `\example` directory.
   1. The files in the `\example` directory can be used as a starting point for your project; however, they are not required to use the REFPROP wrapper.
   2. As part of the build process, if only the example directory is downloaded, its `CMakeLists.txt` file contains a routine to download this repository's contents to a subfolder `/external/refprop/`.
8. If not using the example files and the associated `CMakeLists.txt` file, add a call to `add_subdirectory(refprop)` in your own `CMakeLists.txt` file, where `refprop` is the path to the REFPROP wrapper directory.
9. Run CMake from this directory to generate the build files.
    1. For Visual Studio on Windows, this can be accomplished by either utilizing the `CMakePresets.json` file  or creating and running the following batch file (e.g., `build.bat`).
       ```
       rmdir /Q/S build_windows
       mkdir build_windows
       cd build_windows
    
       cmake -G "Visual Studio 17 2022" -DCMAKE_CONFIGURATION_TYPES="Debug;Release" ..
    
       pause
       ```
    2. For Linux, the following shell script can be used (e.g., `build.sh`).
       ```
       #!/bin/bash
 
       rm -rf build_linux
       mkdir build_linux
       cd build_linux
    
       cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
    
       make
       ```
    **NOTE**: If CMake does not properly fetch the [manyso](https://github.com/usnistgov/manyso) repository, you can manually clone it into `/external/manyso/`, or `/external/refprop/external/manyso` if using the example, and then re-run CMake.
10. The `main.cpp` file in the `/src` directory in the example code shows how to call various REFPROP functions.


Alternative
---
REFPROP can also be called using the header defining the library at: [REFPROP-headers](https://github.com/CoolProp/REFPROP-headers). This header includes functions for runtime loading of the DLL (shared library). Present there are also examples of how to use it and a CMake-based build system. This is an older approach that is not recommended for new projects.

Help
-----
Please file an issue at https://github.com/usnistgov/REFPROP-cpp-wrapper/issues and we will get back to you as quickly as possible. Please don't email NIST staff directly, the only exception being if you have proprietary code that cannot be shared publicly.