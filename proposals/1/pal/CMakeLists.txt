cmake_minimum_required(VERSION 3.5.0)

project(pal)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


add_library(pal SHARED
  src/pal.cpp
)

target_include_directories(pal
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_subdirectory(examples)