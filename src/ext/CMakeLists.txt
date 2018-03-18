cmake_minimum_required(VERSION 3.2)

execute_process(COMMAND git submodule update --init --recursive)

# sdl2 #########################
add_subdirectory(sdl2)
target_include_directories(SDL2main PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdl2/include>)
target_include_directories(SDL2 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdl2/include>)
################################

# glm ##########################
add_subdirectory(glm)
################################

# glew #########################
add_subdirectory(glew/build/cmake)
target_include_directories(glew PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glew/include>)
################################

# zlib #########################
add_subdirectory(zlib)
target_include_directories(zlib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/zlib>)
################################

# png ##########################
set(PNG_BUILD_ZLIB ON CACHE BOOL "Build zlib" FORCE)
set(ZLIB_LIBRARY $<TARGET_FILE:zlib>)
set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib" "${CMAKE_CURRENT_BINARY_DIR}/zlib")
add_subdirectory(png)
if(TARGET png_static)
  add_dependencies(png_static zlib)
endif()
if(TARGET png_framework)
  add_dependencies(png_framework zlib)
endif()
if(TARGET png)
  add_dependencies(png zlib)
endif()
if(TARGET png-fix-itxt)
  add_dependencies(png-fix-itxt zlib)
endif()
target_include_directories(png PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/png>)
target_include_directories(png PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/png>)
################################

# jpeg ##########################
set(BUILD_STATIC OFF CACHE BOOL "Build static jpeg" FORCE)
add_subdirectory(jpeg)
target_include_directories(jpeg PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/jpeg>)
target_include_directories(jpeg PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/jpeg>)
################################

# sdl2_image ###################
add_subdirectory(sdl2_image)
################################
