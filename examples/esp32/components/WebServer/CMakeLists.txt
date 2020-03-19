idf_component_register(
    SRCS "WebServer.cpp" "HTTPRequest.cpp"
    INCLUDE_DIRS "."
    REQUIRES "esp_https_server" "esp_http_server" "MicroSDC"
    EMBED_TXTFILES "certs/cacert.pem"
                   "certs/serverCert.pem"
                   "certs/serverKey.pem"
    )
