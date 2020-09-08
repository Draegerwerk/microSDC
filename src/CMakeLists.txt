
set(HEADERS
    "datamodel/BICEPS_MessageModel.hpp"
    "datamodel/BICEPS_ParticipantModel.hpp"
    "datamodel/ExpectedElement.hpp"
    "datamodel/MDPWSConstants.hpp"
    "datamodel/MessageModel.hpp"
    "datamodel/MessageSerializer.hpp"
    "datamodel/ws-MetadataExchange.hpp"
    "datamodel/ws-addressing.hpp"
    "datamodel/ws-discovery.hpp"
    "datamodel/ws-dpws.hpp"
    "datamodel/ws-eventing.hpp"
    "datamodel/xs_duration.hpp"

    "discovery/DiscoveryService.hpp"
    "discovery/MessagingContext.hpp"

    "networking/NetworkConfig.hpp"

    "services/DeviceService.hpp"
    "services/GetService.hpp"
    "services/ServiceInterface.hpp"
    "services/SetService.hpp"
    "services/SoapFault.hpp"
    "services/SoapService.hpp"
    "services/StateEventService.hpp"
    "services/StaticService.hpp"

    "uuid/UUID.hpp"
    "uuid/UUIDGenerator.hpp"

    "wsdl/GetServiceWSDL.hpp"
    "wsdl/SetServiceWSDL.hpp"
    "wsdl/StateEventServiceWSDL.hpp"

    "DeviceCharacteristics.hpp"
    "Log.hpp"
    "MetadataProvider.hpp"
    "MicroSDC.hpp"
    "SDCConstants.hpp"
    "StateHandler.hpp"
    "SubscriptionManager.hpp"

    "WebServer/Request.hpp"
    "WebServer/WebServer.hpp"

    "SessionManager/SessionManager.hpp"
    )


set(SOURCES
    "datamodel/BICEPS_MessageModel.cpp"
    "datamodel/BICEPS_ParticipantModel.cpp"
    "datamodel/ExpectedElement.cpp"
    "datamodel/MessageModel.cpp"
    "datamodel/MessageSerializer.cpp"
    "datamodel/ws-addressing.cpp"
    "datamodel/ws-discovery.cpp"
    "datamodel/ws-dpws.cpp"
    "datamodel/ws-eventing.cpp"
    "datamodel/ws-MetadataExchange.cpp"
    "datamodel/xs_duration.cpp"

    "discovery/DiscoveryService.cpp"
    "discovery/MessagingContext.cpp"

    "networking/NetworkConfig.cpp"

    "services/DeviceService.cpp"
    "services/GetService.cpp"
    "services/SetService.cpp"
    "services/StateEventService.cpp"
    "services/SoapService.cpp"
    "services/StaticService.cpp"

    "uuid/UUID.cpp"
    "uuid/UUIDGenerator.cpp"

    "DeviceCharacteristics.cpp"
    "Log.cpp"
    "MetadataProvider.cpp"
    "MicroSDC.cpp"
    "StateHandler.cpp"
    "SubscriptionManager.cpp"

    "WebServer/Request.cpp"

    "SessionManager/SessionManager.cpp"
    )

add_library(microSDC_common OBJECT ${SOURCES} ${HEADERS})
target_include_directories(microSDC_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(microSDC_common PUBLIC rapidxml)
