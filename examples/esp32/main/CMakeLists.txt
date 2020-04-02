idf_component_register(
    SRCS "main.cpp"
    )

option(BUILD_EXAMPLES OFF)

add_subdirectory(../../../ microSDC)

target_link_libraries(${COMPONENT_LIB} PUBLIC microSDC)