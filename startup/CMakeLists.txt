cmake_minimum_required(VERSION 3.24)


add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::Main)

if(ANDROID)
    set(files [[]])
    find_library(lib NAMES ${CMAKE_ANDROID_STL_TYPE} ${ANDROID_STL})
    if (lib AND lib MATCHES [[.*\.so$]])
        list(APPEND files ${lib})
    endif()

    if(files)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND_EXPAND_LISTS COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${files} $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()
endif()

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
)


if(ANDROID AND CMAKE_CROSSCOMPILING_EMULATOR)
    file(DOWNLOAD
        https://www.nuget.org/api/v2/package/android-exe-launcher/33.6.0
        ${CMAKE_BINARY_DIR}/android-exe-launcher.nupkg
        EXPECTED_HASH SHA512=8c10a17bbdaf0d312b8925761d07cf28000c8009a3db589a2c211b61ec1ff26605da25bdfb94a8f4198f474fdf4e3fc931a1682fb288e389a960f214596cb7a6
    )

    file(ARCHIVE_EXTRACT
        INPUT ${CMAKE_BINARY_DIR}/android-exe-launcher.nupkg
        DESTINATION ${CMAKE_BINARY_DIR}/android-exe-launcher
    )
endif()




project(${CMAKE_PROJECT_NAME}Main)

set(GFLAGS_USE_TARGET_NAMESPACE ON)
find_package(gflags 2.2.2 REQUIRED)
find_package(glog 0.6 REQUIRED)


add_library(${PROJECT_NAME} OBJECT main.cpp redirect.c)
add_library(${CMAKE_PROJECT_NAME}::Main ALIAS ${PROJECT_NAME})
set_property(SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS VERSION=\"${CMAKE_PROJECT_VERSION}\")
target_link_libraries(${PROJECT_NAME} PRIVATE gflags::gflags glog::glog $<$<BOOL:${ANDROID}>:log>)



project(${CMAKE_PROJECT_NAME}JNI)

if(NOT CMAKE_ANDROID_API)
    if(ANDROID_NATIVE_API_LEVEL)
        set(CMAKE_ANDROID_API ${ANDROID_NATIVE_API_LEVEL})
    elseif(CMAKE_SYSTEM_VERSION)
        set(CMAKE_ANDROID_API ${CMAKE_SYSTEM_VERSION})
    endif()
endif()

find_package(JNI)

if(JNI_FOUND)
    add_library(${PROJECT_NAME} STATIC jni.cpp)
    add_library(${CMAKE_PROJECT_NAME}::JNI ALIAS ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} PRIVATE JNI::JNI ${CMAKE_PROJECT_NAME}::Main)
endif()

