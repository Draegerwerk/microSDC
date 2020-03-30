cmake_minimum_required(VERSION 3.10)

project(MicroSDC VERSION 0.2 DESCRIPTION "An SDC IEEE 11073 Implementation for micro controllers")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

add_subdirectory(src/)

# include doxygen documentation to cmake
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(DOXYGEN_EXCLUDE_PATTERNS "${PROJECT_SOURCE_DIR}/src/rapidxml")

    doxygen_add_docs(Documentation
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/src"
        COMMENT "Generating API documentation with Doxygen"
    )
endif()
