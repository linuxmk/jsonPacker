cmake_minimum_required(VERSION 3.10)

# set the project name
project(jsonPacker)


# add the executable
add_executable(jsonPacker main.c tlv.c)


target_link_libraries(jsonPacker json-c apr-1 aprutil-1)
