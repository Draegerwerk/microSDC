#pragma once

#include <string>

/// @brief DeviceCharacteristics holds general information describing this medical device as
/// specified by BICEPS
class DeviceCharacteristics
{
public:
  void set_manufacturer(const std::string& name);
  void set_manufacturer_url(const std::string& url);
  void set_model_name(const std::string& name);
  void set_presentation_url(const std::string& url);

  const std::string& get_manufacturer() const;
  const std::string& get_manufacturer_url() const;
  const std::string& get_model_name() const;
  const std::string& get_presentation_url() const;

  void set_friendly_name(const std::string& name);
  void set_firmware_version(const std::string& version);
  void set_serial_number(const std::string& serial_number);

  const std::string& get_friendly_name() const;
  const std::string& get_firmware_version() const;
  const std::string& get_serial_number() const;

  void set_endpoint_reference(const std::string& epr);
  const std::string& get_endpoint_reference() const;

private:
  // ThisModel
  std::string manufacturer_;
  std::string manufacturer_url_;
  std::string model_name_;
  std::string presentation_url_;

  // ThisDevice
  std::string friendly_name_;
  std::string firmware_version_;
  std::string serial_number_;

  // Host
  std::string endpoint_reference_;
};
