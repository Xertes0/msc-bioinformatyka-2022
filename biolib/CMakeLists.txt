if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    add_executable(bio)
    set(CMAKE_EXECUTABLE_SUFFIX ".mjs")
else()
    add_library(bio)
endif()

if(COTIRE_ENABLED)
    cotire(bio)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(bio PUBLIC -fconstexpr-steps=10485760)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(bio PUBLIC -fconstexpr-backtrace-limit=0)
    endif()
endif()

target_include_directories(bio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/skeletal_main.cc)
target_sources(bio PRIVATE ${SOURCES})

target_link_options(bio PRIVATE -lembind)

target_link_libraries(
    bio PRIVATE
    range-v3::range-v3
    gcem::gcem
)

if(DOXYGEN_ENABLED)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    doxygen_add_docs(biolib_docs ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    add_executable(skeletal ${CMAKE_CURRENT_SOURCE_DIR}/src/skeletal_main.cc)
    target_link_libraries(skeletal PRIVATE bio)
endif()
