cmake_minimum_required(VERSION 3.10)

add_compile_options("-DRAPIDXML_STATIC_POOL_SIZE=1024")
add_compile_options("-DRAPIDXML_DYNAMIC_POOL_SIZE=1024")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(MicroSDC)
