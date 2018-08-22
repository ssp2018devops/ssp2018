cmake_minimum_required(VERSION 3.5.0)

project(gal)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


add_library(gal SHARED
  src/gal.cpp
)

target_compile_options(gal PRIVATE
  -std=c++17
)

target_include_directories(gal
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_subdirectory(examples)