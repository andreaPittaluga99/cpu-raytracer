cmake_minimum_required(VERSION 3.20)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(RayTracer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build unit tests" ON)

find_package(OpenMP REQUIRED)

add_executable(raytracer src/main.cpp)
target_include_directories(raytracer PRIVATE include)

target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_CXX)

if(MSVC)
    target_compile_options(raytracer PRIVATE /W4 /O2)
else()
    target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic -O3)
endif()

if(BUILD_TESTS)
    include(FetchContent)
    
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    include(GoogleTest)

    # --- Test Vec3 ---
    add_executable(test_vec3 test/vec3_test.cpp)
    target_include_directories(test_vec3 PRIVATE include)
    target_link_libraries(test_vec3 PRIVATE gtest_main)
    
    # --- Test Ray ---
    add_executable(test_ray test/ray_test.cpp)
    target_include_directories(test_ray PRIVATE include)
    target_link_libraries(test_ray PRIVATE gtest_main)

    if(MSVC)
        target_compile_options(test_vec3 PRIVATE /W4)
        target_compile_options(test_ray PRIVATE /W4)
    else()
        target_compile_options(test_vec3 PRIVATE -Wall -Wextra -Wpedantic)
        target_compile_options(test_ray PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    gtest_discover_tests(test_vec3)
    gtest_discover_tests(test_ray)
endif()

# --- Target benchmark ---
add_executable(bench_raytracer src/benchmark.cpp)
target_include_directories(bench_raytracer PRIVATE include)
target_link_libraries(bench_raytracer PRIVATE OpenMP::OpenMP_CXX)

if(MSVC)
    target_compile_options(bench_raytracer PRIVATE /W4 /O2)
else()
    target_compile_options(bench_raytracer PRIVATE -Wall -Wextra -Wpedantic -O3)
endif()