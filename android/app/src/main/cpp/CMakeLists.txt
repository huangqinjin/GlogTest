cmake_minimum_required(VERSION 3.16.0)
project(native-lib)

add_library(${PROJECT_NAME} SHARED native-lib.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::JNI log)

