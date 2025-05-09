## Developing on Windows

1. Install the required dependencies (see `/CMakeLists.txt`). [vcpkg](https://github.com/microsoft/vcpkg) is recommended.
1.
   ```
   git clone https://github.com/hwabis/soosh-native.git
   cd soosh-native
   mkdir build
   cd build
   ```
1. Choose one of:
   - Visual Studio:
   ```cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/your/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake```
   
   Open the `.sln` in Visual Studio, and run one of the targets.
   - Ninja:
   ```
   cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/your/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static-md -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
   cmake --build .
   ```

   Ninja is preferred for `clang-tidy` support with Visual Studio Code and the `clangd` extension.
   Note that as of this writing, there are some [annoying](https://github.com/microsoft/vcpkg/issues/44730) [bugs](https://github.com/microsoft/vcpkg/issues/25300) that require workarounds:
   - Install `x64-windows-static-md` versions of packages through vcpkg
   - Remove all `-ignore:4221` from `vcpkg/installed/x64-windows-static-md/share/absl/abslTargets.cmake`
