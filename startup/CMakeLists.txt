cmake_minimum_required(VERSION 3.24)


add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::Main)




project(${CMAKE_PROJECT_NAME}Main)

set(GFLAGS_USE_TARGET_NAMESPACE ON)
find_package(gflags 2.2.2 REQUIRED)
find_package(glog 0.6 REQUIRED)


add_library(${PROJECT_NAME} OBJECT main.cpp redirect.c)
add_library(${CMAKE_PROJECT_NAME}::Main ALIAS ${PROJECT_NAME})
set_property(SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS VERSION=\"${CMAKE_PROJECT_VERSION}\")
target_link_libraries(${PROJECT_NAME} PRIVATE gflags::gflags glog::glog)




project(${CMAKE_PROJECT_NAME}JNI)

find_package(JNI)

if(JNI_FOUND)
    add_library(${PROJECT_NAME} STATIC jni.cpp)
    add_library(${CMAKE_PROJECT_NAME}::JNI ALIAS ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} PRIVATE JNI::JNI ${CMAKE_PROJECT_NAME}::Main)
endif()

