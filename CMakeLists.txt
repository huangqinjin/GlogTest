cmake_minimum_required(VERSION 3.21)
project(GlogTest VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)


if(ANDROID)
    set(INSTALL_PATH ${CMAKE_CURRENT_LIST_DIR}/libs/android)
    list(PREPEND CMAKE_FIND_ROOT_PATH ${INSTALL_PATH}/${ANDROID_ABI}-root)
    add_subdirectory(android)
endif()


add_subdirectory(libs)
add_subdirectory(startup)
