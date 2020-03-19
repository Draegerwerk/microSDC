set(SOURCES
    "src/datamodel/BICEPS_MessageModel.cpp"
    "src/datamodel/BICEPS_ParticipantModel.cpp"
    "src/datamodel/ExpectedElement.cpp"
    "src/datamodel/MessageModel.cpp"
    "src/datamodel/MessageSerializer.cpp"
    "src/datamodel/ws-addressing.cpp"
    "src/datamodel/ws-discovery.cpp"
    "src/datamodel/ws-dpws.cpp"
    "src/datamodel/ws-eventing.cpp"
    "src/datamodel/ws-MetadataExchange.cpp"
    "src/datamodel/xs_duration.cpp"

    "src/dpws/DPWSHost.cpp"
    "src/dpws/MessagingContext.cpp"
    "src/dpws/MetadataProvider.cpp"

    "src/networking/NetworkConfig.cpp"

    "src/services/DeviceService.cpp"
    "src/services/GetService.cpp"
    "src/services/Request.cpp"
    "src/services/SetService.cpp"
    "src/services/StateEventService.cpp"
    "src/services/SoapService.cpp"
    "src/services/StaticService.cpp"

    "src/uuid/UUID.cpp"
    "src/uuid/UUIDGenerator.cpp"

    "src/DeviceCharacteristics.cpp"
    "src/Log.cpp"
    "src/MicroSDC.cpp"
    "src/StateHandler.cpp"
    "src/SubscriptionManager.cpp"
    )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS "./src/" REQUIRES "asio")
