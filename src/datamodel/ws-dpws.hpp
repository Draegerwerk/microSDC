#pragma once

#include "ws-addressing.hpp"
#include "ws-discovery.hpp"
#include <string>
#include <vector>

namespace WS::DPWS
{
  struct ThisModelType
  {
    // Manufacturer
    //
    using ManufacturerType = std::string;
    using ManufacturerSequence = std::vector<ManufacturerType>;
    ManufacturerSequence manufacturer;

    // ManufacturerUrl
    //
    using ManufacturerUrlType = WS::ADDRESSING::URIType;
    using ManufacturerUrlOptional = std::optional<ManufacturerUrlType>;
    ManufacturerUrlOptional manufacturerUrl;

    // ModelName
    //
    using ModelNameType = std::string;
    using ModelNameSequence = std::vector<ModelNameType>;
    ModelNameSequence modelName;

    // ModelNumber
    //
    using ModelNumberType = std::string;
    using ModelNumberOptional = std::optional<ModelNumberType>;
    ModelNumberOptional modelNumber;

    // ModelUrl
    //
    using ModelUrlType = WS::ADDRESSING::URIType;
    using ModelUrlOptional = std::optional<ModelUrlType>;
    ModelUrlOptional modelUrl;

    // PresentationUrl
    //
    using PresentationUrlType = WS::ADDRESSING::URIType;
    using PresentationUrlOptional = std::optional<PresentationUrlType>;
    PresentationUrlOptional presentationUrl;
  };

  struct ThisDeviceType
  {
    // FriendlyName
    //
    using FriendlyNameType = std::string;
    using FriendlyNameSequence = std::vector<FriendlyNameType>;
    FriendlyNameSequence friendlyName;

    // FirmwareVersion
    //
    using FirmwareVersionType = std::string;
    using FirmwareVersionOptional = std::optional<FirmwareVersionType>;
    FirmwareVersionOptional firmwareVersion;

    // SerialNumber
    //
    using SerialNumberType = std::string;
    using SerialNumberOptional = std::optional<SerialNumberType>;
    SerialNumberOptional serialNumber;
  };
  struct HostServiceType
  {
    // EndpointReference
    //
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpointReference;

    // Types
    //
    using TypesType = WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    TypesOptional types;

    explicit HostServiceType(EndpointReferenceType epr);
  };
  struct HostedServiceType
  {
    // EndpointReference
    //
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;
    using EndpointReferenceSequence = std::vector<EndpointReferenceType>;
    EndpointReferenceSequence endpointReference;

    // Types
    //
    using TypesType = WS::DISCOVERY::QNameListType;
    TypesType types;

    // ServiceId
    //
    using ServiceIdType = WS::ADDRESSING::URIType;
    ServiceIdType serviceId;

    HostedServiceType(EndpointReferenceSequence epr, TypesType types, ServiceIdType serviceId);
  };
  struct Relationship
  {
    // Host
    //
    using HostType = WS::DPWS::HostServiceType;
    HostType host;

    // Hosted
    //
    using HostedType = WS::DPWS::HostedServiceType;
    using HostedSequence = std::vector<HostedType>;
    HostedSequence hosted;

    // Type
    //
    using TypeType = std::string;
    TypeType type;

    Relationship(HostType host, HostedSequence hosted, TypeType type);
  };
} // namespace WS::DPWS
