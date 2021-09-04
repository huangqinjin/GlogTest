cmake_minimum_required(VERSION 3.21)

file(GLOB patches ${CMAKE_CURRENT_LIST_DIR}/patches/${CMAKE_ARGV3}-*.patch)

if(NOT patches)
    return()
endif()

find_package(Git REQUIRED QUIET)

list(SORT patches)
foreach(p IN LISTS patches)

    cmake_path(GET p FILENAME name)

    execute_process(
        COMMAND ${GIT_EXECUTABLE} apply --check -R ${p}
        RESULT_VARIABLE result
        OUTPUT_QUIET ERROR_QUIET
    )

    if(NOT result)
        message(STATUS "${name} has already been applied.")
        continue()
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} apply ${p}
        RESULT_VARIABLE result
    )

    if(NOT result)
        message(STATUS "${name} has successfully been applied.")
    else()
        message(FATAL_ERROR "${name} failed to be applied.")
    endif()

endforeach()

