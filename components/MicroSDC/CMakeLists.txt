set(COMPONENT_SOURCES
        "datamodel/ExpectedElement.cpp"
        "datamodel/MessageModel.cpp"
        "datamodel/MessageSerializer.cpp"
        "datamodel/ws-addressing.cpp"
        "datamodel/ws-discovery.cpp"
        "datamodel/ws-dpws.cpp"
        "datamodel/ws-MetadataExchange.cpp"
        "dpws/DPWSHost.cpp"
        "dpws/MessagingContext.cpp"
        "dpws/MetadataProvider.cpp"
        "DeviceCharacteristics.cpp"
        "DeviceService.cpp"
        "GetService.cpp"
        "MicroSDC.cpp"
        "SoapService.cpp"
        "StaticService.cpp"
    )

idf_component_register(
    SRCS ${COMPONENT_SOURCES}
    INCLUDE_DIRS "."
    REQUIRES "WebServer" "UUID" "Networking" "rapidxml" "asio" "pthread"
    )
