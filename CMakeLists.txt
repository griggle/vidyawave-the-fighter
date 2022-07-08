cmake_minimum_required(VERSION 3.12)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(Vidyawave_The_Fighter)

add_definitions(-w)

# third party libs

# SDL2
if (WIN32)
    set(SDL2_DIR deps/SDL2-vc)
    set(SDL2_image_DIR deps/SDL2_image-vc)
    set(SDL2_net_DIR deps/SDL2_net-vc)
    set(SDL2_mixer_DIR deps/SDL2_mixer-vc)
else()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/deps/cmake_modules")
endif (WIN32)

find_package(SDL2 REQUIRED)
message(SDL2_INCLUDE_DIRS="${SDL2_INCLUDE_DIRS}")
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
message(SDL2_IMAGE_INCLUDE_DIRS="${SDL2_IMAGE_INCLUDE_DIRS}")
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

find_package(SDL2_net REQUIRED)
message(SDL2_NET_INCLUDE_DIRS="${SDL2_NET_INCLUDE_DIRS}")
include_directories(${SDL2_NET_INCLUDE_DIRS})

find_package(SDL2_mixer REQUIRED)
message(SDL2_MIXER_INCLUDE_DIRS="${SDL2_MIXER_INCLUDE_DIRS}")
include_directories(${SDL2_MIXER_INCLUDE_DIRS})

#######################################################################################################################
#                                                       CLIENT                                                        #
#######################################################################################################################

add_executable(Vidyawave_The_Fighter "")

# todo: figure out how do switch builds on debug / release
#if(CMAKE_BUILD_TYPE EQUAL "Debug")
#    add_executable(Vidyawave_The_Fighter src/main.cpp)
#else()
#    add_executable(Vidyawave_The_Fighter WIN32 src/main.cpp)
#endif(CMAKE_BUILD_TYPE EQUAL "Release")

add_subdirectory(src)
target_sources(Vidyawave_The_Fighter 
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/generated/generated_state_atlas.cpp
        ${CMAKE_CURRENT_LIST_DIR}/generated/generated_state_hitboxes.cpp
        ${CMAKE_CURRENT_LIST_DIR}/generated/generated_state_hurtboxes.cpp
)
target_include_directories(Vidyawave_The_Fighter PRIVATE include)

message(SDL2_LIBRARIES="${SDL2_LIBRARIES}")
#set(SDL2_LIBRARIES /usr/lib/x86_64-linux-gnu/libSDL2main.a/usr/lib/x86_64-linux-gnu/libSDL2.so-lpthread)
target_link_libraries(Vidyawave_The_Fighter ${SDL2_LIBRARIES})
message(SDL2_IMAGE_LIBRARIES="${SDL2_IMAGE_LIBRARIES}")
target_link_libraries(Vidyawave_The_Fighter ${SDL2_IMAGE_LIBRARIES})
message(SDL2_NET_LIBRARIES="${SDL2_NET_LIBRARIES}")
target_link_libraries(Vidyawave_The_Fighter ${SDL2_NET_LIBRARIES})
message(SDL2_MIXER_LIBRARIES="${SDL2_MIXER_LIBRARIES}")
target_link_libraries(Vidyawave_The_Fighter ${SDL2_MIXER_LIBRARIES})


if (WIN32)
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_DIR}/lib/x64" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x64" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_net_DIR}/lib/x64" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_mixer_DIR}/lib/x64" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

else ()
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_DIR}/lib/x86" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_image_DIR}/lib/x86" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_net_DIR}/lib/x86" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_mixer_DIR}/lib/x86" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )
endif ()
    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/res" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>/res 
            /xd "files" "__pycache__" /xf "*.py" "*.cmd" "info.png" > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET Vidyawave_The_Fighter PRE_BUILD 
        COMMAND (python "${PROJECT_SOURCE_DIR}/scripts/atlas_generator.py")
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET Vidyawave_The_Fighter PRE_BUILD 
        COMMAND (python "${PROJECT_SOURCE_DIR}/scripts/hitbox_generator.py")
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

else ()

    add_custom_command(TARGET Vidyawave_The_Fighter POST_BUILD 
        COMMAND cp -r
            "${PROJECT_SOURCE_DIR}/res" 
            $<TARGET_FILE_DIR:Vidyawave_The_Fighter>
    )

    add_custom_command(TARGET Vidyawave_The_Fighter PRE_BUILD 
        COMMAND (python3 "${PROJECT_SOURCE_DIR}/scripts/atlas_generator.py")
    )

    add_custom_command(TARGET Vidyawave_The_Fighter PRE_BUILD 
        COMMAND (python3 "${PROJECT_SOURCE_DIR}/scripts/hitbox_generator.py")
    )

endif(WIN32)

#######################################################################################################################
#                                                       SERVER                                                        #
#######################################################################################################################

add_executable(VtF_Server "src/networking/server_main.cpp")

target_include_directories(VtF_Server PRIVATE include)

target_link_libraries(VtF_Server ${SDL2_LIBRARIES})
target_link_libraries(VtF_Server ${SDL2_NET_LIBRARIES})


if (WIN32)
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    add_custom_command(TARGET VtF_Server POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_DIR}/lib/x64" 
            $<TARGET_FILE_DIR:VtF_Server> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET VtF_Server POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_net_DIR}/lib/x64" 
            $<TARGET_FILE_DIR:VtF_Server> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

else ()
    add_custom_command(TARGET VtF_Server POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_DIR}/lib/x86" 
            $<TARGET_FILE_DIR:VtF_Server> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )

    add_custom_command(TARGET VtF_Server POST_BUILD 
        COMMAND (robocopy /s /xo 
            "${PROJECT_SOURCE_DIR}/${SDL2_net_DIR}/lib/x86" 
            $<TARGET_FILE_DIR:VtF_Server> > nul)
            ^& IF %ERRORLEVEL% LEQ 1 verify >nul
    )
endif ()
endif(WIN32)