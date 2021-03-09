#include "DeviceCharacteristics.hpp"

// ThisModel
//
void DeviceCharacteristics::set_manufacturer(const std::string& name)
{
  manufacturer_ = name;
}
void DeviceCharacteristics::set_manufacturer_url(const std::string& url)
{
  manufacturer_url_ = url;
}
void DeviceCharacteristics::set_model_name(const std::string& name)
{
  model_name_ = name;
}
void DeviceCharacteristics::set_presentation_url(const std::string& url)
{
  presentation_url_ = url;
}

const std::string& DeviceCharacteristics::get_manufacturer() const
{
  return manufacturer_;
}
const std::string& DeviceCharacteristics::get_manufacturer_url() const
{
  return manufacturer_url_;
}
const std::string& DeviceCharacteristics::get_model_name() const
{
  return model_name_;
}
const std::string& DeviceCharacteristics::get_presentation_url() const
{
  return presentation_url_;
}

// ThisDevice
//
void DeviceCharacteristics::set_friendly_name(const std::string& name)
{
  friendly_name_ = name;
}
void DeviceCharacteristics::set_firmware_version(const std::string& version)
{
  firmware_version_ = version;
}
void DeviceCharacteristics::set_serial_number(const std::string& serial_number)
{
  serial_number_ = serial_number;
}

const std::string& DeviceCharacteristics::get_friendly_name() const
{
  return friendly_name_;
}
const std::string& DeviceCharacteristics::get_firmware_version() const
{
  return firmware_version_;
}
const std::string& DeviceCharacteristics::get_serial_number() const
{
  return serial_number_;
}

// Host
//
void DeviceCharacteristics::set_endpoint_reference(const std::string& epr)
{
  endpoint_reference_ = epr;
}

const std::string& DeviceCharacteristics::get_endpoint_reference() const
{
  return endpoint_reference_;
}
