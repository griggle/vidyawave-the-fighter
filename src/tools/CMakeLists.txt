#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/scenes)

target_sources(Vidyawave_The_Fighter 
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/sdl_image_tools.cpp
        ${CMAKE_CURRENT_LIST_DIR}/animation.cpp
        ${CMAKE_CURRENT_LIST_DIR}/hit_animation.cpp
)