cmake_minimum_required (VERSION 3.12.1)
set (CMAKE_CXX_STANDARD 17)
project (MyProject)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(main ${SOURCES})
