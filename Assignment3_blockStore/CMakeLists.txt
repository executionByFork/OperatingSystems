cmake_minimum_required (VERSION 2.8)
project(assignment3)

set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror")

# Read some documentation, do some googling, refer to things we've given you
# cmake is popular, so someone on the internet has had your problem before


include_directories("${CMAKE_SOURCE_DIR}/include")

# if the test program isn't found in the correct place, you will lose points

add_library(libblock_store SHARED
    src/bitmap.c
    src/block_store.c
)
add_executable(${PROJECT_NAME}_test test/tests.cpp)

# Enable grad/bonus tests by setting the variable to 1
target_compile_definitions(${PROJECT_NAME}_test PRIVATE GRAD_TESTS=0)

target_link_libraries(${PROJECT_NAME}_test libblock_store gtest pthread)

enable_testing()
add_test(NAME    ${PROJECT_NAME}_test
         COMMAND ${PROJECT_NAME}_test)