cmake_minimum_required(VERSION 3.11)

option(TESTS_ENABLED "Build unit tests" ON)
option(DOXYGEN_ENABLED "Build doxygen documentation" ON)
option(CONAN_ENABLED "Use conan package manager" ON)
option(COTIRE_ENABLED "Use cotire" ON)
option(LTO_ENABLED "Enable lto for Release builds" OFF)
option(EMSCRIPTEN_EVAL_CTORS "Enable -sEVAL_CTORS for emscripten" OFF)

project(msc-bioinformatyka-2022)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

if(LTO_ENABLED AND CMAKE_BUILD_TYPE STREQUAL "Release")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT LTO_SUPPORTED)

    if(LTO_SUPPORTED)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION  TRUE)
    else()
        message(WARNING "Lto is enabled but not supported")
    endif()
endif()

if(COTIRE_ENABLED)
    include(cmake/Cotire.cmake)
endif()

if(TESTS_ENABLED)
    if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
        message(WARNING "Tests are enabled (TESTS_ENABLED) but emscripten is used. Disabling.")
        set(TESTS_ENABLED OFF)
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    add_compile_options(-Werror -Wno-error=unknown-pragmas -Wall -Wextra -Wshadow -Wpedantic -Wconversion)

    add_compile_options (-fsanitize=address,leak,undefined)
    add_link_options    (-fsanitize=address,leak,undefined)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
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

    if(EMSCRIPTEN_EVAL_CTORS)
        add_link_options(
            -sEVAL_CTORS=2
        )
    endif()

    add_subdirectory(web)
endif()

if(CONAN_ENABLED)
    include(cmake/Conan.cmake)
endif()

find_package(range-v3 REQUIRED)
find_package(gcem REQUIRED)

add_subdirectory(biolib)

if(DOXYGEN_ENABLED)
    include(cmake/DoxygenAwesome.cmake)
    find_package(Doxygen REQUIRED)

    add_subdirectory(docs)
endif()

if(TESTS_ENABLED)
    enable_testing()
    find_package(Catch2 REQUIRED)
    add_subdirectory(tests)
endif()
