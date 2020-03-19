#include "DeviceCharacteristics.hpp"

// ThisModel
//
void DeviceCharacteristics::setManufacturer(const std::string& name)
{
  manufacturer_ = name;
}
void DeviceCharacteristics::setManufacturerUrl(const std::string& url)
{
  manufacturerUrl_ = url;
}
void DeviceCharacteristics::setModelName(const std::string& name)
{
  modelName_ = name;
}
void DeviceCharacteristics::setPresentationUrl(const std::string& url)
{
  presentationUrl_ = url;
}

const std::string& DeviceCharacteristics::getManufacturer() const
{
  return manufacturer_;
}
const std::string& DeviceCharacteristics::getManufacturerUrl() const
{
  return manufacturerUrl_;
}
const std::string& DeviceCharacteristics::getModelName() const
{
  return modelName_;
}
const std::string& DeviceCharacteristics::getPresentationUrl() const
{
  return presentationUrl_;
}

// ThisDevice
//
void DeviceCharacteristics::setFriendlyName(const std::string& name)
{
  friendlyName_ = name;
}
void DeviceCharacteristics::setFirmwareVersion(const std::string& version)
{
  firmwareVersion_ = version;
}
void DeviceCharacteristics::setSerialNumber(const std::string& serialNumber)
{
  serialNumber_ = serialNumber;
}

const std::string& DeviceCharacteristics::getFriendlyName() const
{
  return friendlyName_;
}
const std::string& DeviceCharacteristics::getFirmwareVersion() const
{
  return firmwareVersion_;
}
const std::string& DeviceCharacteristics::getSerialNumber() const
{
  return serialNumber_;
}

// Host
//
void DeviceCharacteristics::setEndpointReference(const std::string& epr)
{
  endpointReference_ = epr;
}

const std::string& DeviceCharacteristics::getEndpointReference() const
{
  return endpointReference_;
}
