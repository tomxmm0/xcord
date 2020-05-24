cmake_minimum_required(VERSION 3.6.0 FATAL_ERROR)

project(xcord CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB XCORD_SOURCE "src/**.cpp" "src/**.hpp")
file(GLOB XCORD_HEADERS "include/**.hpp" "include/xcord/**.hpp")

add_library(xcord SHARED ${XCORD_SOURCE} ${XCORD_HEADERS})

include_directories(include)

add_compile_definitions(BUILDING_XCORD)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(xcord PRIVATE cpr)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(xcord PRIVATE fmt::fmt)

include_directories(deps/rapidjson/include)
include_directories(deps/websocketpp)
include_directories(deps/asio/asio/include)

add_compile_definitions(ASIO_STANDALONE _WEBSOCKETPP_CPP11_RANDOM_DEVICE_ _WEBSOCKETPP_CPP11_TYPE_TRAITS_)

if (ENABLE_UNITTEST)
    add_subdirectory(unittest)
endif()
