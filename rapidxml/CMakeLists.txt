# rapidxml

add_library(rapidxml INTERFACE)

target_compile_definitions(rapidxml INTERFACE RAPIDXML_STATIC_POOL_SIZE=1024)
target_compile_definitions(rapidxml INTERFACE RAPIDXML_DYNAMIC_POOL_SIZE=1024)
target_include_directories(rapidxml INTERFACE .)
