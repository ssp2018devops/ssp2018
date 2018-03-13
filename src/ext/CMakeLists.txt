cmake_minimum_required(VERSION 3.2)

# SDL2 #########################
add_subdirectory(sdl2)
add_library(sdl2 STATIC IMPORTED)
set_target_properties(sdl2 PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/bin/sdl2main"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/sdl2/include"
)
add_dependencies(sdl2 SDL2main)
################################
