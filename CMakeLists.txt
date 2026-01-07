cmake_minimum_required(VERSION 3.10)
project(OSLAB4)

set(CMAKE_CXX_STANDARD 11)

add_executable(OSLABA4 OSLABA4.cpp shared_utils.cpp)
add_executable(sender sender.cpp shared_utils.cpp)

target_include_directories(OSLABA4 PRIVATE .)
target_include_directories(sender PRIVATE .)