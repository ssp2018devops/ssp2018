cmake_minimum_required(VERSION 3.2)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Please run cmake out of tree. E.g. create a separate build directory and run cmake from there.")
endif()

add_subdirectory(ext)
add_subdirectory(3d_project)
add_subdirectory(ThisIsAClass)
  