Include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.2.0
)

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

add_executable(
    tests

    biolib/example.cc
    biolib/translate.cc
)

target_link_libraries(tests PRIVATE bio Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)