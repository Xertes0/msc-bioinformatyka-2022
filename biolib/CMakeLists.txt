if(CMAKE_BUILD_TYPE STREQUAL "Emscripten")
    add_executable(bio)
else()
    add_library(bio)
endif()

target_include_directories(bio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
target_sources(bio PRIVATE ${SOURCES})

target_link_options(bio PRIVATE -lembind)