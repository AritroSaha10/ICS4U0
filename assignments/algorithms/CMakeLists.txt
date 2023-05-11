cmake_minimum_required(VERSION 3.10)
project(Algorithms)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 32fd1453194c37d5136577ee55629b863a42333f)
FetchContent_MakeAvailable(cpr)

include_directories("${PROJECT_SOURCE_DIR}")

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        "${PROJECT_SOURCE_DIR}/include/vehicles/*.h"
        "${PROJECT_SOURCE_DIR}/include/vehicles/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/vehicles/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/vehicles/*.c"
        )

add_executable(Algorithms ${all_SRCS})
target_link_libraries(Algorithms PRIVATE cpr::cpr)
# target_link_libraries(DataStructures SHARED)
