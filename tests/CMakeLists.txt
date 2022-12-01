add_executable(
    tests

    biolib/example.cc
    biolib/translate.cc
)

if(COTIRE_ENABLED)
    cotire(tests)
endif()

target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
    bio
)

include(CTest)
include(Catch)

catch_discover_tests(tests)