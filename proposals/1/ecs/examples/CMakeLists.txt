cmake_minimum_required(VERSION 3.5.0)


add_executable(ecs-example-1
  main.cpp
)


target_link_libraries(ecs-example-1
  PUBLIC
    ecs
)