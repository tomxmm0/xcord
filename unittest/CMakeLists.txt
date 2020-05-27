cmake_minimum_required(VERSION 3.6.0 FATAL_ERROR)

project(unittest CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB UNITTEST_SOURCE "**.cpp" "**.hpp")

add_executable(unittest ${UNITTEST_SOURCE})

include_directories(../include)

include_directories(../deps/rapidjson/include)
include_directories(../deps/websocketpp)
include_directories(../deps/asio/asio/include)

add_compile_definitions(ASIO_STANDALONE _WEBSOCKETPP_CPP11_RANDOM_DEVICE_ _WEBSOCKETPP_CPP11_TYPE_TRAITS_)
