# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

add_compile_options("-DRAPIDXML_STATIC_POOL_SIZE=1024")
add_compile_options("-DRAPIDXML_DYNAMIC_POOL_SIZE=1024")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(MicroSDC)

if(CONFIG_BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()
