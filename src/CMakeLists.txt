
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

    "dpws/DPWSHost.hpp"
    "dpws/MessagingContext.hpp"
    "dpws/MetadataProvider.hpp"

    "networking/NetworkConfig.hpp"

    "rapidxml/rapidxml.hpp"
    "rapidxml/rapidxml_print.hpp"

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

    "dpws/DPWSHost.cpp"
    "dpws/MessagingContext.cpp"
    "dpws/MetadataProvider.cpp"

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
    "MicroSDC.cpp"
    "StateHandler.cpp"
    "SubscriptionManager.cpp"

    "WebServer/Request.cpp"
    )

add_library(microSDC OBJECT ${SOURCES} ${HEADERS})
target_include_directories(microSDC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
