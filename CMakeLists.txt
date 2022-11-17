cmake_minimum_required(VERSION 3.8)

enable_testing()

project(msc-bioinformatyka-2022)

set(CMAKE_CXX_STANDARD 20)

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
    add_link_options(${COMPILE_AND_LINK_OPTIONS})
endif()

add_subdirectory(biolib)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    add_subdirectory(tests)
endif()
