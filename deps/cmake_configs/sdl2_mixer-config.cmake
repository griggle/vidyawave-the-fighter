set(SDL2_mixer_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_mixer_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2_mixer.lib")
else ()
  set(SDL2_mixer_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2_mixer.lib")
endif ()

string(STRIP "${SDL2_mixer_LIBRARIES}" SDL2_mixer_LIBRARIES)