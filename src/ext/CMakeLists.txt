cmake_minimum_required(VERSION 3.2)

# sdl2 #########################
add_subdirectory(sdl2)
target_include_directories(SDL2main PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdl2/include>)
################################

# glm #########################
add_subdirectory(glm)
################################

# glew #########################
add_subdirectory(glew/build/cmake)
target_include_directories(glew PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glew/include>)
################################