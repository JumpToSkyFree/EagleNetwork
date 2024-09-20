project(EagleNetworkTests)

set(
    EAGLE_NET_TESTS_SOURCES
    ./ResultTests.cc
    ./ResourceInitializerTests.cc
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(
    EagleNetworkTests
    ${EAGLE_NET_TESTS_SOURCES}
)

target_link_libraries(
    EagleNetworkTests
    EagleNetwork
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(EagleNetworkTests)
