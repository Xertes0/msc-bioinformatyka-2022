cmake_minimum_required(VERSION 3.8)

option(TESTS_ENABLED "Build unit tests" ON)
option(DOXYGEN_ENABLED "Build doxygen documentation" ON)

project(msc-bioinformatyka-2022)

set(CMAKE_CXX_STANDARD 20)

if(DOXYGEN_ENABLED)
    find_package(Doxygen REQUIRED)
endif()

if(TESTS_ENABLED)
    if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
        message(WARNING "Tests are enabled (TESTS_ENABLED) but emscripten is used. Disabling.")
        set(TESTS_ENABLED OFF)
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Wall -Wextra -Wshadow -Wpedantic -Wconversion)

    add_compile_options (-fsanitize=address,leak,undefined)
    add_link_options    (-fsanitize=address,leak,undefined)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    message(STATUS "Building for emscripten")

    add_compile_definitions(EMSCRIPTEN)

    set(COMPILE_AND_LINK_OPTIONS
        -fwasm-exceptions # Enable only on debug builds?
    )

    add_compile_options(${COMPILE_AND_LINK_OPTIONS})
    add_link_options(${COMPILE_AND_LINK_OPTIONS}
        -sMODULARIZE=1
        -sEXPORT_ES6=1
    )
endif()

include(cmake/Conan.cmake)

add_subdirectory(biolib)

if (TESTS_ENABLED)
    enable_testing()
    add_subdirectory(tests)
endif()
