cmake_minimum_required(VERSION 3.10)

project(Euler2D_B LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")

add_subdirectory(src)
