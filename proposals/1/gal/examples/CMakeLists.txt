cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


add_executable(basic
  basic.cpp
)

target_link_libraries(basic 
  PUBLIC
    gal
)

add_executable(stream
  stream.cpp
)

target_link_libraries(stream 
  PUBLIC
    gal
)