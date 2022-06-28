add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/states)

target_include_directories(Vidyawave_The_Fighter PRIVATE include)

target_sources(Vidyawave_The_Fighter 
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/player.cpp
        ${CMAKE_CURRENT_LIST_DIR}/john_debug.cpp
)