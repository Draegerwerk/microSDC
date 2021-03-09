#pragma once

#include "DeviceCharacteristics.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/ws-MetadataExchange.hpp"

class NetworkConfig;

/// @brief MetadataProvider provides any metadata information to SDC services
class MetadataProvider
{
public:
  using Host = WS::DPWS::HostServiceType;
  using Hosted = WS::DPWS::HostedServiceType;
  using HostedSequence = WS::DPWS::Relationship::HostedSequence;
  using MetadataSection = WS::MEX::MetadataSection;

  /// @brief constructs a MetadataProvider object from given Device Characteristics
  /// @param networkConfig the network configuration of MicroSDC
  /// @param devChar Device Characteristics to provide with this MetadataProvider
  MetadataProvider(std::shared_ptr<const NetworkConfig> network_config,
                   DeviceCharacteristics dev_char);

  /// @brief get the URI of the Device Service
  /// @return string containing the URI
  static std::string get_device_service_path();

  /// @brief get the URI of the GetService
  /// @return string containing the URI
  static std::string get_get_service_path();

  /// @brief get the URI of the SetService
  /// @return string containing the URI
  static std::string get_set_service_path();

  /// @brief get the URI of the StateEventService
  /// @return string containing the URI
  static std::string get_state_event_service_path();

  /// @brief gets the endpoint URI of the SetService
  /// @return URI containing information regarding the set service
  WS::ADDRESSING::URIType get_set_service_uri() const;

  /// @brief gets the endpoint URI of the StateEventService
  /// @return URI containing information regarding the state event service
  WS::ADDRESSING::URIType get_state_event_service_uri() const;

  /// @brief fill SOAP Envelope with Device Metadata for GetMetadataResponse
  /// @param envelope reference to the response envelope to fill
  void fill_device_metadata(MESSAGEMODEL::Envelope& envelope) const;

  /// @brief fill SOAP Envelope with GetService Metadata for GetMetadataResponse
  /// @param envelope reference to the response envelope to fill
  void fill_get_service_metadata(MESSAGEMODEL::Envelope& envelope) const;

  /// @brief fill SOAP Envelope with SetService Metadata for GetMetadataResponse
  /// @param envelope reference to the response envelope to fill
  void fill_set_service_metadata(MESSAGEMODEL::Envelope& envelope) const;

  /// @brief fill SOAP Envelope with StateEventService Metadata for GetMetadataResponse
  /// @param envelope reference to the response envelope to fill
  void fill_state_event_service_metadata(MESSAGEMODEL::Envelope& envelope) const;

  /// @brief compile host part of MetadataSection Relationship
  /// @return Host section
  Host create_host_metadata() const;

  /// @brief compile MetadataSection ThisModel containing information like e.g. manufacturer
  /// @return filled ThisModel section
  MetadataSection create_metadata_section_this_model() const;

  /// @brief compile MetadataSection ThisDevice containing information like e.g. friendly name
  /// @return filled ThisDevice section
  MetadataSection create_metadata_section_this_device() const;

  /// @brief compile MetadataSection Relationship connecting Host with Hosted Services
  /// @param host Host section custructed by createHostMetadata()
  /// @param hosted sequence of Hosted services
  /// @return filled Relationship section
  static MetadataSection create_netadata_section_relationship(const Host& host,
                                                              const HostedSequence& hosted);

  /// @brief compile information about WSDL for GetService
  /// @return constructed MetadataSection
  MetadataSection create_metadata_section_wsdl_get_service() const;

  /// @brief compile information about WSDL for SetService
  /// @return constructed MetadataSection
  MetadataSection create_metadata_section_wsdl_set_service() const;

  /// @brief compile information about WSDL for StateEventService
  /// @return constructed MetadataSection
  MetadataSection create_metadata_section_wsdl_state_event_service() const;

  /// @brief construct Hosted section for GetService
  /// @brief Hosted element
  Hosted create_hosted_get_service() const;

  /// @brief construct Hosted section for SetService
  /// @brief Hosted element
  Hosted create_hosted_set_service() const;

  /// @brief construct Hosted section for StateEventService
  /// @brief Hosted element
  Hosted create_hosted_state_event_service() const;

private:
  /// network configuration of this device
  const std::shared_ptr<const NetworkConfig> network_config_;
  /// device characteristics to provide
  const DeviceCharacteristics device_characteristics_;
};
