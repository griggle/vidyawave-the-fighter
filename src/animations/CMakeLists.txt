#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/scenes)

target_sources(Vidyawave_The_Fighter 
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/animation.cpp
        ${CMAKE_CURRENT_LIST_DIR}/h_animation.cpp
     
)