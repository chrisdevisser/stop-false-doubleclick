cmake_minimum_required(VERSION 3.7)
project(stop_false_doubleclick)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DUNICODE)

add_executable(stop_false_doubleclick main.cpp)
set_target_properties(stop_false_doubleclick PROPERTIES LINK_FLAGS "-Wl,--subsystem,windows")
