## Developing on Windows

1. Install the required dependencies (see `/CMakeLists.txt`). [vcpkg](https://github.com/microsoft/vcpkg) is recommended.
1.
   ```
   git clone https://github.com/hwabis/soosh-native.git
   cd soosh-native
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/your/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   ```
   This uses the Visual Studio generator by default. (Note that you can use the Ninja generator for VSCode + clangd integration, but it's extremely finicky on Windows.)
