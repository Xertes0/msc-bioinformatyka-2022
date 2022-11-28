list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/.local)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(
        DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake"
        TLS_VERIFY ON
    )
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

set(TESTS_REQUIRES)
if(TESTS_ENABLED)
    list(APPEND TESTS_REQUIRES catch2/3.2.0)
endif()

set(CONAN_ADD_OPTIONS)
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    list(APPEND CONAN_ADD_OPTIONS fmt:header_only=True)
endif()

conan_cmake_configure(
    REQUIRES range-v3/0.12.0
             fmt/9.1.0
             ${TESTS_REQUIRES}
    GENERATORS cmake_find_package cmake_paths
    OPTIONS ${CONAN_ADD_OPTIONS}
)

conan_cmake_autodetect(conan_settings)

conan_cmake_install(
    PATH_OR_REFERENCE .
    BUILD missing
    OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/conan_output
    SETTINGS ${conan_settings}
)