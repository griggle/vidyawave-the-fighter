#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/scenes)

target_sources(Vidyawave_The_Fighter 
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/scene.cpp
        ${CMAKE_CURRENT_LIST_DIR}/game_scene.cpp
        ${CMAKE_CURRENT_LIST_DIR}/online_game_scene.cpp
)