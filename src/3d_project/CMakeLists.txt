cmake_minimum_required(VERSION 3.2)

project (3d CXX)
include(CheckCXXCompilerFlag)

file(GLOB_RECURSE SRC "src/*.cpp")
add_executable(3d main.cpp ${SRC})

target_include_directories(3d PRIVATE "incl")


CHECK_CXX_COMPILER_FLAG(-std=c++17 HAS_FLAG_STD)
if (HAS_FLAG_STD)
  target_compile_options(3d PRIVATE -std=c++17)
else()
  CHECK_CXX_COMPILER_FLAG(/std=c++17 HAS_FLAG_STD)
  if (HAS_FLAG_STD)
    target_compile_options(3d PRIVATE /std=c++17)
  endif()
endif()

target_compile_definitions(3d PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_link_libraries(3d SDL2main glm glew sdl2_image)#SDL2 SDL2main SDL2_image GL dl pthread)

add_custom_command(TARGET 3d POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glew> 
                                                    $<TARGET_FILE:png> 
                                                    $<TARGET_FILE:sdl2_image> 
                                                    $<TARGET_FILE:SDL2> 
                                                    $<TARGET_FILE:zlib> 
                                                    ${CMAKE_CURRENT_BINARY_DIR})

                                                  
set(ASSETS_LINK ${CMAKE_CURRENT_BINARY_DIR}/assets)
set(SHADERS_LINK ${CMAKE_CURRENT_BINARY_DIR}/shaders)
set(3D_ASSETS_DIR ${CMAKE_SOURCE_DIR}/../assets/3d_project) 
set(ASSETS_DIR ${3D_ASSETS_DIR}/assets)
set(SHADERS_DIR ${3D_ASSETS_DIR}/shaders)
                                                  
if(WIN32)
  add_custom_command(TARGET 3d COMMAND mklink /J "\"${ASSETS_LINK}\"" "\"${ASSETS_DIR}\""
                               COMMAND mklink /J "\"${SHADERS_LINK}\"" "\"${SHADERS_DIR}\"")
else()
  add_custom_command(TARGET 3d COMMAND ${CMAKE_COMMAND} -E create_symlink "\"${ASSETS_DIR}\"" "\"${ASSETS_LINK}\""
                               COMMAND ${CMAKE_COMMAND} -E create_symlink "\"${SHADERS_DIR}\"" "\"${SHADERS_LINK}\"")
endif()