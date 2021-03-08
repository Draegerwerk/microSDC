idf_component_register(
    SRCS "main.cpp" "SimpleDevice.cpp"
    )

option(BUILD_EXAMPLES OFF)

add_subdirectory(../../../ microSDC)

target_link_libraries(${COMPONENT_LIB} PUBLIC microSDC)
target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_17)