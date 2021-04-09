cmake_minimum_required(VERSION 3.10)

# set the project name
project(zlatko)


# add the executable
add_executable(zlatko main.c tlv.c jsonPacker.c)

if (CMAKE_BUILD_TYPE EQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -NDEBUG")
endif()

target_link_libraries(zlatko json-c apr-1 aprutil-1)
