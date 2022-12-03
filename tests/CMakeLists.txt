add_executable(
    tests

    biolib/example.cc
    biolib/translate.cc
    biolib/skeletal/detail.cc
)

target_link_libraries(
    tests PRIVATE
    Catch2::Catch2WithMain
    bio
)

if(COTIRE_ENABLED)
    cotire(tests)
endif()

include(CTest)
include(Catch)

catch_discover_tests(tests)