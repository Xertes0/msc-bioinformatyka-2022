if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    add_executable(bio)
    set(CMAKE_EXECUTABLE_SUFFIX ".mjs")
else()
    add_library(bio)
endif()

target_include_directories(bio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
target_sources(bio PRIVATE ${SOURCES})

target_link_options(bio PRIVATE -lembind)

target_link_libraries(
    bio PRIVATE
    CONAN_PKG::range-v3
)

if(DOXYGEN_ENABLED)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    doxygen_add_docs(biolib_docs ${CMAKE_CURRENT_SOURCE_DIR})
endif()