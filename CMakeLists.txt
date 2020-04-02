cmake_minimum_required(VERSION 3.10)

project(MicroSDC VERSION 0.2 DESCRIPTION "An SDC IEEE 11073 Implementation for micro controllers")

option(BUILD_EXAMPLES "Build the examples for linux targets" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

# rapidxml interface library
add_subdirectory(rapidxml/)

# Add microSDC library target objects
add_subdirectory(src/)

set(PORTS_LINUX_SOURCES
    ports/linux/ClientSession.linux.cpp
    ports/linux/Request.linux.cpp
    ports/linux/WebServer.linux.cpp
    )

set(PORTS_LINUX_HEADERS
    ports/linux/ClientSession.linux.hpp
    ports/linux/Request.linux.hpp
    ports/linux/WebServer.linux.hpp
    )

set(PORTS_ESP_SOURCES
    ports/esp/ClientSession.esp.cpp
    ports/esp/Request.esp.cpp
    ports/esp/WebServer.esp.cpp
    )

set(PORTS_ESP_HEADERS
    ports/esp/ClientSession.esp.hpp
    ports/esp/Request.esp.hpp
    ports/esp/WebServer.esp.hpp
    )

if (UNIX)
    message("Configuring linux target...")
    # Dependency WebServer
    add_subdirectory(Simple-Web-Server/)
    add_library(microSDC ${PORTS_LINUX_SOURCES} ${PORTS_LINUX_HEADERS} $<TARGET_OBJECTS:microSDC_common>)
    target_include_directories(microSDC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/ports/linux)
    target_link_libraries(microSDC simple-web-server rapidxml)
elseif(ESP_PLATFORM)
    message("Configuring esp target...")
    cmake_policy(SET CMP0079 NEW)
    target_link_libraries(microSDC_common PRIVATE idf::asio)
    add_library(microSDC ${PORTS_ESP_SOURCES} ${PORTS_ESP_HEADERS} $<TARGET_OBJECTS:microSDC_common>)
    target_include_directories(microSDC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/ports/esp)
    target_link_libraries(microSDC idf::esp_https_server idf::esp_http_client rapidxml)
else()
    message(SEND_ERROR "Platform not supported!")
endif()

if(BUILD_EXAMPLES)
    message("Configuring examples...")
    add_subdirectory(examples)
endif()

# include doxygen documentation to cmake
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    doxygen_add_docs(Documentation
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/src"
        COMMENT "Generating API documentation with Doxygen"
    )
endif()
