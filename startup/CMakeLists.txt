cmake_minimum_required(VERSION 3.21)


add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::Main)




project(${CMAKE_PROJECT_NAME}Main)

set(GFLAGS_USE_TARGET_NAMESPACE ON)
find_package(gflags 2.2.2 REQUIRED)
find_package(glog 0.5 REQUIRED)


add_library(${PROJECT_NAME} OBJECT main.cpp)
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


if(JNI_INCLUDE_DIRS)
    add_library(${PROJECT_NAME} STATIC jni.cpp)
    add_library(${CMAKE_PROJECT_NAME}::JNI ALIAS ${PROJECT_NAME})
    target_include_directories(${PROJECT_NAME} PRIVATE ${JNI_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::Main)
endif()

