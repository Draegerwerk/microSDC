#pragma once

#include "ws-addressing.hpp"
#include "ws-discovery.hpp"
#include <string>
#include <vector>

namespace WS::DPWS
{
  class ThisModelType
  {
  public:
    // Manufacturer
    //
    using ManufacturerType = std::string;
    using ManufacturerSequence = std::vector<ManufacturerType>;
    const ManufacturerSequence& Manufacturer() const;
    ManufacturerSequence& Manufacturer();

    // ManufacturerUrl
    //
    using ManufacturerUrlType = WS::ADDRESSING::URIType;
    using ManufacturerUrlOptional = std::optional<ManufacturerUrlType>;

    // ModelName
    //
    using ModelNameType = std::string;
    using ModelNameSequence = std::vector<ModelNameType>;
    const ModelNameSequence& ModelName() const;
    ModelNameSequence& ModelName();

    // ModelNumber
    //
    using ModelNumberType = std::string;
    using ModelNumberOptional = std::optional<ModelNumberType>;

    // ModelUrl
    //
    using ModelUrlType = WS::ADDRESSING::URIType;
    using ModelUrlOptional = std::optional<ModelUrlType>;

    // PresentationUrl
    //
    using PresentationUrlType = WS::ADDRESSING::URIType;
    using PresentationUrlOptional = std::optional<PresentationUrlType>;

  protected:
    ManufacturerSequence Manufacturer_;
    ManufacturerUrlOptional ManufacturerUrl_;
    ModelNameSequence ModelName_;
    ModelNumberOptional ModelNumber_;
    ModelUrlOptional ModelUrl_;
    PresentationUrlOptional PresentationUrl_;
  };
  class ThisDeviceType
  {
  public:
    // FriendlyName
    //
    using FriendlyNameType = std::string;
    using FriendlyNameSequence = std::vector<FriendlyNameType>;
    const FriendlyNameSequence& FriendlyName() const;
    FriendlyNameSequence& FriendlyName();

    // FirmwareVersion
    //
    using FirmwareVersionType = std::string;
    using FirmwareVersionOptional = std::optional<FirmwareVersionType>;

    // SerialNumber
    //
    using SerialNumberType = std::string;
    using SerialNumberOptional = std::optional<SerialNumberType>;

  protected:
    FriendlyNameSequence FriendlyName_;
    FirmwareVersionOptional FirmwareVersion_;
    SerialNumberOptional SerialNumber_;
  };
  class HostServiceType
  {
  public:
    // EndpointReference
    //
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;
    const EndpointReferenceType& EndpointReference() const;
    EndpointReferenceType& EndpointReference();

    // Types
    //
    using TypesType = WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    const TypesOptional& Types() const;
    TypesOptional& Types();

    HostServiceType(EndpointReferenceType epr);

  protected:
    EndpointReferenceType EndpointReference_;
    TypesOptional Types_;
  };
  class HostedServiceType
  {
  public:
    // EndpointReference
    //
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;
    using EndpointReferenceSequence = std::vector<EndpointReferenceType>;
    const EndpointReferenceSequence& EndpointReference() const;
    EndpointReferenceSequence& EndpointReference();

    // Types
    //
    using TypesType = WS::DISCOVERY::QNameListType;
    const TypesType& Types() const;
    TypesType& Types();

    // ServiceId
    //
    using ServiceIdType = WS::ADDRESSING::URIType;
    const ServiceIdType& ServiceId() const;
    ServiceIdType& ServiceId();

    HostedServiceType(EndpointReferenceSequence epr, TypesType types, ServiceIdType serviceId);

  protected:
    EndpointReferenceSequence EndpointReference_;
    TypesType Types_;
    ServiceIdType ServiceId_;
  };
  class Relationship
  {
  public:
    // Host
    //
    using HostType = WS::DPWS::HostServiceType;
    const HostType& Host() const;
    HostType& Host();

    // Hosted
    //
    using HostedType = WS::DPWS::HostedServiceType;
    using HostedSequence = std::vector<HostedType>;
    const HostedSequence& Hosted() const;
    HostedSequence& Hosted();

    // Type
    //
    using TypeType = std::string;
    const TypeType& Type() const;
    TypeType& Type();

    Relationship(HostType host, HostedSequence hosted, TypeType type);

  protected:
    HostType Host_;
    HostedSequence Hosted_;
    TypeType Type_;
  };
} // namespace WS::DPWS
