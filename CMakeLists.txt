cmake_minimum_required(VERSION 3.12)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(Vidyawave_The_Fighter)

# third party libs

# SDL2
if (WIN32)
    set(SDL2_DIR deps/SDL2-vc)
    set(SDL2_image_DIR deps/SDL2_image-vc)
    set(SDL2_net_DIR deps/SDL2_net-vc)
endif (WIN32)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_image_INCLUDE_DIRS})

find_package(SDL2_net REQUIRED)
include_directories(${SDL2_net_INCLUDE_DIRS})

# add the executable
add_executable(Vidyawave_The_Fighter "")

add_subdirectory(src)
target_include_directories(Vidyawave_The_Fighter PRIVATE include)


# todo: figure out how do switch builds on debug / release
#if(CMAKE_BUILD_TYPE EQUAL "Debug")
#    add_executable(Vidyawave_The_Fighter src/main.cpp)
#else()
#    add_executable(Vidyawave_The_Fighter WIN32 src/main.cpp)
#endif(CMAKE_BUILD_TYPE EQUAL "Release")

target_link_libraries(Vidyawave_The_Fighter ${SDL2_LIBRARIES})
target_link_libraries(Vidyawave_The_Fighter ${SDL2_image_LIBRARIES})
target_link_libraries(Vidyawave_The_Fighter ${SDL2_net_LIBRARIES})


if (WIN32)
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    # SDL2
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_DIR}/lib/x64/SDL2.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>)    

    # SDL2 image        
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x64/SDL2_image.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x64/zlib1.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x64/libjpeg-9.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x64/libpng16-16.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x64/libtiff-5.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x64/libwebp-7.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 

    # SDL2 net
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_net_DIR}/lib/x64/SDL2_net.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
else ()
    # SDL2
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_DIR}/lib/x86/SDL2.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>)   

    # SDL2 image
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x86/SDL2_image.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x86/zlib1.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x86/libjpeg-9.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x86/libpng16-16.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x86/libtiff-5.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x86/libwebp-7.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 

    # SDL2 net
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different            
            "${PROJECT_SOURCE_DIR}/${SDL2_net_DIR}/lib/x86/SDL2_net.dll"      
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>) 
endif ()

    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo
            "${PROJECT_SOURCE_DIR}/res" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>/res) ^& IF %ERRORLEVEL% LEQ 1 exit 0
    )

else ()

    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND cp -r
            "${PROJECT_SOURCE_DIR}/res" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>
    )

endif(WIN32)