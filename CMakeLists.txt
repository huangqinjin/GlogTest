cmake_minimum_required(VERSION 3.24)
project(GlogTest VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE PATH ".dll, .so, .dylib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE PATH ".dll, .exe")


if(ANDROID)
    set(INSTALL_PATH ${CMAKE_CURRENT_LIST_DIR}/libs/android)
    list(PREPEND CMAKE_FIND_ROOT_PATH ${INSTALL_PATH}/${ANDROID_ABI}-root)
    add_subdirectory(android)
endif()


add_subdirectory(libs)

enable_testing()
add_subdirectory(startup)
