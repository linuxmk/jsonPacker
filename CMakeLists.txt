cmake_minimum_required(VERSION 3.10)

# set the project name
project(jsonParser)


# add the executable
add_executable(jsonParser main.c hashmap.c)


target_link_libraries(jsonParser json-c apr-1 aprutil-1)
