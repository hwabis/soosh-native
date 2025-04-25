## Developing on Windows

1. Ensure you have:
   - Visual Studio 2022 (or Visual Studio Build Tools)
   - CMake
   - vcpkg
1. Install [vcpkg](https://github.com/microsoft/vcpkg), and install required dependencies (see `CMakeLists.txt`)
1. Generate the build:
   ```bash
   git clone https://github.com/hwabis/soosh-native.git
   cd soosh-native
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/your/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   ```
1. Open the `.sln` in Visual Studio 2022 and run a target (or `cmake --build .` then run the executables).

Visual Studio 2022 has built-in `clang-tidy` support, but there's no way (that I know of) to cleanly aggregate all the results. (Also note that the code is not fully `clang-tidy` compliant).
The recommended method is using WSL2 and something like:
   ```bash
   mkdir build-wsl
   cd build-wsl
   cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
   cmake --build .
   cd ..
   ./run_clang_tidy.sh
   ```
