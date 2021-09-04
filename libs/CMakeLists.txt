cmake_minimum_required(VERSION 3.21)

include(FetchContent)

set(PATCH ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/patch.cmake)

FetchContent_Declare(gflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    GIT_TAG        v2.2.2
    GIT_SUBMODULES ""
    PATCH_COMMAND ${PATCH} gflags
    OVERRIDE_FIND_PACKAGE
)
FetchContent_Declare(glog
    GIT_REPOSITORY https://github.com/google/glog.git
    GIT_TAG        v0.5.0
    PATCH_COMMAND ${PATCH} glog
    OVERRIDE_FIND_PACKAGE
)


set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_SKIP_INSTALL_RULES ON)
set(BUILD_TESTING OFF)

set(GFLAGS_BUILD_SHARED_LIBS ON)
set(GFLAGS_BUILD_STATIC_LIBS OFF)
set(GFLAGS_BUILD_gflags_LIB ON)
set(GFLAGS_BUILD_gflags_nothreads_LIB OFF)
set(GFLAGS_USE_TARGET_NAMESPACE ON)

set(WITH_GTEST OFF)
set(WITH_PKGCONFIG OFF)


FetchContent_MakeAvailable(gflags glog)

