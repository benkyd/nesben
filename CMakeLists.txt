cmake_minimum_required(VERSION 3.20)

project(NESBEN VERSION 0.0.1 LANGUAGES C CXX)

# --------------------------------------------------------------------
# Language Settings
# --------------------------------------------------------------------

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Keep downloaded dependencies in the build directory
set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/vendor" CACHE PATH "" FORCE)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.18       # Or whatever version you prefer
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui
    PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui
    PUBLIC
        SDL3::SDL3
)

# Could ImGUI be any more annoying
target_compile_options(imgui PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-w>
)

file(GLOB_RECURSE NESBEN_SOURCES
    CONFIGURE_DEPENDS
    src/*.c
    src/*.cc
    src/*.cpp
    src/*.cxx
)

add_executable(NESBEN ${NESBEN_SOURCES})

target_include_directories(NESBEN
    PRIVATE
        src
)

target_link_libraries(NESBEN
    PRIVATE
        SDL3::SDL3
        imgui
)
