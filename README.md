# soosh-native

CLI-based implementation of [Sushi Go!](https://boardgamegeek.com/boardgame/133473/sushi-go) over plain TCP (Boost.Asio).

## Developing on Windows

Install the required dependencies (see `/CMakeLists.txt`). [vcpkg](https://github.com/microsoft/vcpkg) is recommended. Then run:
   ```
   git clone https://github.com/hwabis/soosh-native.git
   cd soosh-native
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/your/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build .
   ```
