if (NOT EXISTS "${CMAKE_BINARY_DIR}/doxygen-awesome.css")
    message(STATUS "Downloading doxygen-awesome.css from https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/v2.1.0/doxygen-awesome.css")
    file(
            DOWNLOAD "https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/v2.1.0/doxygen-awesome.css"
            "${CMAKE_BINARY_DIR}/doxygen-awesome.css"
            TLS_VERIFY ON
    )
endif ()
