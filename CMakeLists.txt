cmake_minimum_required(VERSION 4.1)
project(untitled C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)

FetchContent_MakeAvailable(raylib)

add_executable(untitled main.c
        playlist.c
        playlist.h
        playlist.h
        playlist.c
        playlist.c
        playlist.h)

target_link_libraries(untitled raylib)

if(MSVC)
    target_link_libraries(untitled winmm)
elseif(MINGW)
    target_link_libraries(untitled winmm gdi32)
endif()

