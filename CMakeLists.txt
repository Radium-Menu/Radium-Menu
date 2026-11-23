cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_SYSTEM_NAME STREQUAL "iOS" OR IOS)
    message(FATAL_ERROR "Radium does not support iOS builds.")
endif()

set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(Radium VERSION 0.1.0)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR
        "Unable to find Geode SDK! Please define the GEODE_SDK environment variable to point to Geode."
    )
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory(
    $ENV{GEODE_SDK}
    ${CMAKE_CURRENT_BINARY_DIR}/geode
)

set(IMGUI_ILL_HANDLE_TOUCHES ON CACHE BOOL "" FORCE)

CPMAddPackage(
    "gh:matcool/gd-imgui-cocos#934958c68b1f050a8976d893584fa71368c1df1f"
)

target_link_libraries(${PROJECT_NAME}
    imgui-cocos
)

setup_geode_mod(${PROJECT_NAME})
