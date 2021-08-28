cmake_minimum_required(VERSION 3.21)
project(GlogTest VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)


set(GFLAGS_USE_TARGET_NAMESPACE ON)
find_package(gflags 2.2.2 REQUIRED)
find_package(glog 0.5 REQUIRED)


add_library(${PROJECT_NAME}Main OBJECT main.cpp)
set_property(SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS VERSION=\"${PROJECT_VERSION}\")
target_link_libraries(${PROJECT_NAME}Main PRIVATE gflags::gflags glog::glog)


add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Main)

