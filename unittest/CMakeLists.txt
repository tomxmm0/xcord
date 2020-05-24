file (GLOB UNITTEST_SOURCE "**.cpp" "**.hpp")

add_executable(unittest ${UNITTEST_SOURCE})

include_directories(../include)
target_link_libraries(unittest PRIVATE xcord)
