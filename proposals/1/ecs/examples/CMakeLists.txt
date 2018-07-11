cmake_minimum_required(VERSION 3.5.0)


add_executable(entity_in_sky
  entity_in_sky.cpp
)

target_link_libraries(entity_in_sky
  PUBLIC
    ecs
)

add_executable(gui_events
gui_events.cpp
)

target_link_libraries(gui_events
  PUBLIC
    ecs
)