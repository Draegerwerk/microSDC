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
    ManufacturerUrlOptional manufacturer_url;

    // ModelName
    //
    using ModelNameType = std::string;
    using ModelNameSequence = std::vector<ModelNameType>;
    ModelNameSequence model_name;

    // ModelNumber
    //
    using ModelNumberType = std::string;
    using ModelNumberOptional = std::optional<ModelNumberType>;
    ModelNumberOptional model_number;

    // ModelUrl
    //
    using ModelUrlType = WS::ADDRESSING::URIType;
    using ModelUrlOptional = std::optional<ModelUrlType>;
    ModelUrlOptional model_url;

    // PresentationUrl
    //
    using PresentationUrlType = WS::ADDRESSING::URIType;
    using PresentationUrlOptional = std::optional<PresentationUrlType>;
    PresentationUrlOptional presentation_url;
  };

  struct ThisDeviceType
  {
    // FriendlyName
    //
    using FriendlyNameType = std::string;
    using FriendlyNameSequence = std::vector<FriendlyNameType>;
    FriendlyNameSequence friendly_name;

    // FirmwareVersion
    //
    using FirmwareVersionType = std::string;
    using FirmwareVersionOptional = std::optional<FirmwareVersionType>;
    FirmwareVersionOptional firmware_version;

    // SerialNumber
    //
    using SerialNumberType = std::string;
    using SerialNumberOptional = std::optional<SerialNumberType>;
    SerialNumberOptional serial_number;
  };
  struct HostServiceType
  {
    // EndpointReference
    //
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpoint_reference;

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
    EndpointReferenceSequence endpoint_reference;

    // Types
    //
    using TypesType = WS::DISCOVERY::QNameListType;
    TypesType types;

    // ServiceId
    //
    using ServiceIdType = WS::ADDRESSING::URIType;
    ServiceIdType service_id;

    HostedServiceType(EndpointReferenceSequence epr, TypesType types, ServiceIdType service_id);
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
