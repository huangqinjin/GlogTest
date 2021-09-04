cmake_minimum_required(VERSION 3.21)


file(TOUCH ${PROJECT_BINARY_DIR}/dummy.c)
add_executable(${PROJECT_NAME} ${PROJECT_BINARY_DIR}/dummy.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::Main)




project(${CMAKE_PROJECT_NAME}Main)

set(GFLAGS_USE_TARGET_NAMESPACE ON)
if (NOT TARGET gflags::gflags)
    find_package(gflags 2.2.2 REQUIRED)
endif()

if (NOT TARGET gflags) # https://github.com/google/glog/issues/683
    add_library(gflags ALIAS gflags::gflags)
endif()
if (NOT TARGET glog::glog)
    find_package(glog 0.5 REQUIRED)
endif()

add_library(${PROJECT_NAME} STATIC main.cpp redirect.c)
add_library(${CMAKE_PROJECT_NAME}::Main ALIAS ${PROJECT_NAME})
set_property(SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS VERSION=\"${CMAKE_PROJECT_VERSION}\")
target_link_libraries(${PROJECT_NAME} PRIVATE gflags::gflags glog::glog)




project(${CMAKE_PROJECT_NAME}JNI)

find_package(JNI)

foreach(var IN ITEMS
        JNI_FOUND
        JNI_INCLUDE_DIRS
        JNI_LIBRARIES
        JAVA_INCLUDE_PATH
        JAVA_INCLUDE_PATH2
        JAVA_JVM_LIBRARY
        JAVA_AWT_INCLUDE_PATH
        JAVA_AWT_LIBRARY)
    message(VERBOSE "${var}=[${${var}}]")
endforeach()

if(ANDROID)
    set(JNI_INCLUDE_DIRS ${JAVA_INCLUDE_PATH})
endif()

if(JNI_INCLUDE_DIRS)
    add_library(${PROJECT_NAME} STATIC jni.cpp)
    add_library(${CMAKE_PROJECT_NAME}::JNI ALIAS ${PROJECT_NAME})
    target_include_directories(${PROJECT_NAME} PRIVATE ${JNI_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::Main)
endif()

