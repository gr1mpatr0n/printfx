cmake_minimum_required(VERSION 4.1)
project(crtp)

set(CMAKE_CXX_STANDARD 23)

add_executable(crtp
        main.cpp
        include/printfx.h)

target_include_directories(crtp PUBLIC include)
