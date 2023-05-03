cmake_minimum_required(VERSION 3.10)
project(ExtendedDataStructures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

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

add_executable(ExtendedDataStructures ${all_SRCS})
# target_link_libraries(DataStructures SHARED)
