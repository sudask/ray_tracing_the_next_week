cmake_minimum_required(VERSION 3.25)

project(raytracinginoneweekend)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

aux_source_directory(src SRC)

add_executable(main ${SRC})

include_directories(include)