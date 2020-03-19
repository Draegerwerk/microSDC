cmake_minimum_required(VERSION 3.10)

project(MicroSDC VERSION 0.2 DESCRIPTION "An SDC IEEE 11073 Implementation for micro controllers")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(HEADERS
    "src/datamodel/BICEPS_MessageModel.hpp"
    "src/datamodel/BICEPS_ParticipantModel.hpp"
    "src/datamodel/ExpectedElement.hpp"
    "src/datamodel/MDPWSConstants.hpp"
    "src/datamodel/MessageModel.hpp"
    "src/datamodel/MessageSerializer.hpp"
    "src/datamodel/ws-MetadataExchange.hpp"
    "src/datamodel/ws-addressing.hpp"
    "src/datamodel/ws-discovery.hpp"
    "src/datamodel/ws-dpws.hpp"
    "src/datamodel/ws-eventing.hpp"
    "src/datamodel/xs_duration.hpp"

    "src/dpws/DPWSHost.hpp"
    "src/dpws/MessagingContext.hpp"
    "src/dpws/MetadataProvider.hpp"

    "src/networking/NetworkConfig.hpp"

    "src/rapidxml/rapidxml.hpp"
    "src/rapidxml/rapidxml_print.hpp"

    "src/services/DeviceService.hpp"
    "src/services/GetService.hpp"
    "src/services/Request.hpp"
    "src/services/ServiceInterface.hpp"
    "src/services/SetService.hpp"
    "src/services/SoapFault.hpp"
    "src/services/SoapService.hpp"
    "src/services/StateEventService.hpp"
    "src/services/StaticService.hpp"
    "src/services/WebServerInterface.hpp"

    "src/uuid/UUID.hpp"
    "src/uuid/UUIDGenerator.hpp"

    "src/wsdl/GetServiceWSDL.hpp"
    "src/wsdl/SetServiceWSDL.hpp"
    "src/wsdl/StateEventServiceWSDL.hpp"

    "src/DeviceCharacteristics.hpp"
    "src/Log.hpp"
    "src/MicroSDC.hpp"
    "src/SDCConstants.hpp"
    "src/StateHandler.hpp"
    "src/SubscriptionManager.hpp"
    )


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

add_library(microSDC SHARED ${SOURCES} ${HEADERS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
