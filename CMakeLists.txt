cmake_minimum_required(VERSION 3.17)
project(untitled)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE -O2)

find_package(OpenMP)
set(CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(pirv main.cpp)
target_link_libraries(pirv GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(pirv)