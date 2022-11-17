add_executable(bio)

target_include_directories(bio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
target_sources(bio PRIVATE ${SOURCES})

target_link_options(bio PRIVATE -lembind)