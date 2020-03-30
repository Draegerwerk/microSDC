set(SOURCES
    "WebServer.esp32.cpp"
    "Request.esp32.cpp"
    "SessionManager.esp32.cpp"
    )

idf_component_register(
    SRCS ${SOURCES}
    REQUIRES "esp_https_server" "esp_http_client"
    EMBED_TXTFILES "certs/cacert.pem"
                   "certs/serverCert.pem"
                   "certs/serverKey.pem"
   )

add_subdirectory(src/)
cmake_policy(SET CMP0079 NEW)
target_link_libraries(microSDC PRIVATE idf::asio)

target_include_directories(${COMPONENT_LIB} PUBLIC src/)
target_sources(${COMPONENT_LIB} PUBLIC $<TARGET_OBJECTS:microSDC>)
