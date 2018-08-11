cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(sdl2)
target_include_directories(SDL2main PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdl2/include>)
target_include_directories(SDL2 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdl2/include>)

add_subdirectory(glew/build/cmake)
target_include_directories(glew PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glew/include>)

add_subdirectory(glm)

add_executable(basic
  basic.cpp
)

target_link_libraries(basic 
  PUBLIC
    gal
    SDL2
    glew
    glm
)

add_executable(stream
  stream.cpp
)

target_link_libraries(stream 
  PUBLIC
    gal
)