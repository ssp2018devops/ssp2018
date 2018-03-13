cmake_minimum_required(VERSION 3.2)

project (3d)

# LIBS ###############################
find_library(lib_sdl2 SDL2 "${LIB_SDL2}")
find_library(lib_sdl2main SDL2main "${LIB_SDL2}")
if(NOT lib_sdl2 OR NOT lib_sdl2main)
  message(FATAL_ERROR "SDL2 lib not found. Please specify the absolute library path of SDL2 using '-DLIB_SDL2=<path>'.")
endif()

find_library(lib_sdl2image SDL2_image "${LIB_SDL2}" "${LIB_SDL2_IMAGE}")
if(NOT lib_sdl2image)
  message(FATAL_ERROR "SDL2_image lib not found. Please specify its absolute path using '-DLIB_SDL2_IMAGE=<path>'.")
endif()

find_library(lib_gl GL "${LIB_GL}")
if(NOT lib_gl)
  message(FATAL_ERROR "OpenGL lib not found. Please specify its absolute path using '-DLIB_GL=<path>'.")
endif()

# HEADERS #############################
find_file(incl_sdl2 SDL.h "${INCL_SDL2}")
find_file(incl_sdl2main SDL_main.h "${INCL_SDL2}")
if(NOT incl_sdl2 OR NOT incl_sdl2main)
  message(FATAL_ERROR "SDL2 headers not found. Please specify the absolute headers path of SDL2 using '-DINCL_SDL2=<path>'.")
endif()

find_file(incl_sdl2image SDL_image.h "${INCL_SDL2}" "${INCL_SDL2_IMAGE}")
if(NOT incl_sdl2image)
  message(FATAL_ERROR "SDL2_image headers not found. Please specify its absolute path using '-DINCL_SDL2_IMAGE=<path>'.")
endif()

find_file(incl_gl gl.h "${INCL_GL}")
if(NOT incl_gl)
  message(FATAL_ERROR "OpenGL headers not found. Please specify its absolute path using '-DINCL_GL=<path>'.")
endif()

find_file(incl_glm glm.hpp "${INCL_GLM}")
if(NOT incl_glm)
  message(FATAL_ERROR "GLM headers not found. Please specify its absolute path using '-DINCL_GLM=<path>'.")
endif()


link_directories("${LIB_SDL2}" "${LIB_SDL2_IMAGE}" "${LIB_GL}")

file(GLOB_RECURSE SRC "src/*.cpp")
add_executable(3d main.cpp ${SRC})


target_include_directories(3d PRIVATE "incl" "${INCL_SDL2}/.." "${INCL_SDL2_IMAGE}/.." "${INCL_GL}/.." "${INCL_GLM}/..")
target_compile_options(3d PRIVATE -std=c++17)

target_link_libraries(3d SDL2 SDL2main SDL2_image GL dl pthread)

