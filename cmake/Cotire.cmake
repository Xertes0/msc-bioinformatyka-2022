if(NOT EXISTS "${CMAKE_BINARY_DIR}/cotire.cmake")
    message(STATUS "Downloading cotire.cmake from https://raw.githubusercontent.com/sakra/cotire/cotire-1.8.1/CMake/cotire.cmake")
    file(
        DOWNLOAD "https://raw.githubusercontent.com/sakra/cotire/cotire-1.8.1/CMake/cotire.cmake"
        "${CMAKE_BINARY_DIR}/cotire.cmake"
        TLS_VERIFY ON
    )
endif()

include(${CMAKE_BINARY_DIR}/cotire.cmake)