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

if(TESTS_ENABLED)
    set(TESTS_REQUIRES catch2/3.2.0)
endif()

conan_cmake_configure(
    REQUIRES range-v3/0.12.0
             ${TESTS_REQUIRES}
    GENERATORS cmake_find_package cmake_paths
)

conan_cmake_autodetect(conan_settings)

conan_cmake_install(
    PATH_OR_REFERENCE .
    BUILD missing
    OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/conan_output
    SETTINGS ${conan_settings} compiler.libcxx=libstdc++11
)