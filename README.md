# xcord
Yet another C++ Discord library

### Building

#### Prequesistes 
  * Clone xcord into your folder of choice.
  * Download and configure [cmake](https://cmake.org/) for your operating system.  
  * Download and configure [vcpkg](https://github.com/microsoft/vcpkg) for your operating system.  

In the vcpkg directory, install the following packages:
  * ```vcpkg install openssl```
  * ```vcpkg install curl[http2]```
  * ```vcpkg install cpr```
  * ```vcpkg install fmt```
  
After installing the packages successfully, go to your xcord directory and generate the project with cmake:
```
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```
