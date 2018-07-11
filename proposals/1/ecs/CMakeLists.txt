cmake_minimum_required(VERSION 3.5.0)

project(ecs)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


add_library(ecs SHARED
  impl/World.cpp
  impl/Entity.cpp
  impl/TypeId.cpp
  impl/Key.cpp
  src/EntityManager.cpp
)

target_include_directories(ecs
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)



add_subdirectory(examples)