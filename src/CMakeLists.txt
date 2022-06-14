add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/scenes)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/players)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tools)

target_sources(Vidyawave_The_Fighter 
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/canvas.cpp
)