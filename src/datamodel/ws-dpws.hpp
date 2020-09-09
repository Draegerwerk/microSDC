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
    ManufacturerSequence Manufacturer;

    // ManufacturerUrl
    //
    using ManufacturerUrlType = WS::ADDRESSING::URIType;
    using ManufacturerUrlOptional = std::optional<ManufacturerUrlType>;
    ManufacturerUrlOptional ManufacturerUrl;

    // ModelName
    //
    using ModelNameType = std::string;
    using ModelNameSequence = std::vector<ModelNameType>;
    ModelNameSequence ModelName;

    // ModelNumber
    //
    using ModelNumberType = std::string;
    using ModelNumberOptional = std::optional<ModelNumberType>;
    ModelNumberOptional ModelNumber;

    // ModelUrl
    //
    using ModelUrlType = WS::ADDRESSING::URIType;
    using ModelUrlOptional = std::optional<ModelUrlType>;
    ModelUrlOptional ModelUrl;

    // PresentationUrl
    //
    using PresentationUrlType = WS::ADDRESSING::URIType;
    using PresentationUrlOptional = std::optional<PresentationUrlType>;
    PresentationUrlOptional PresentationUrl;
  };

  struct ThisDeviceType
  {
    // FriendlyName
    //
    using FriendlyNameType = std::string;
    using FriendlyNameSequence = std::vector<FriendlyNameType>;
    FriendlyNameSequence FriendlyName;

    // FirmwareVersion
    //
    using FirmwareVersionType = std::string;
    using FirmwareVersionOptional = std::optional<FirmwareVersionType>;
    FirmwareVersionOptional FirmwareVersion;

    // SerialNumber
    //
    using SerialNumberType = std::string;
    using SerialNumberOptional = std::optional<SerialNumberType>;
    SerialNumberOptional SerialNumber;
  };
  struct HostServiceType
  {
    // EndpointReference
    //
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType EndpointReference;

    // Types
    //
    using TypesType = WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    TypesOptional Types;

    explicit HostServiceType(EndpointReferenceType epr);
  };
  struct HostedServiceType
  {
    // EndpointReference
    //
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;
    using EndpointReferenceSequence = std::vector<EndpointReferenceType>;
    EndpointReferenceSequence EndpointReference;

    // Types
    //
    using TypesType = WS::DISCOVERY::QNameListType;
    TypesType Types;

    // ServiceId
    //
    using ServiceIdType = WS::ADDRESSING::URIType;
    ServiceIdType ServiceId;

    HostedServiceType(EndpointReferenceSequence epr, TypesType types, ServiceIdType serviceId);
  };
  struct Relationship
  {
    // Host
    //
    using HostType = WS::DPWS::HostServiceType;
    HostType Host;

    // Hosted
    //
    using HostedType = WS::DPWS::HostedServiceType;
    using HostedSequence = std::vector<HostedType>;
    HostedSequence Hosted;

    // Type
    //
    using TypeType = std::string;
    TypeType Type;

    Relationship(HostType host, HostedSequence hosted, TypeType type);
  };
} // namespace WS::DPWS
