#pragma once

#include <string>

class DeviceCharacteristics
{
public:
  void setManufacturer(const std::string& name);
  void setManufacturerUrl(const std::string& url);
  void setModelName(const std::string& name);
  void setPresentationUrl(const std::string& url);

  const std::string& getManufacturer() const;
  const std::string& getManufacturerUrl() const;
  const std::string& getModelName() const;
  const std::string& getPresentationUrl() const;

  void setFriendlyName(const std::string& name);
  void setFirmwareVersion(const std::string& version);
  void setSerialNumber(const std::string& serialNumber);

  const std::string& getFriendlyName() const;
  const std::string& getFirmwareVersion() const;
  const std::string& getSerialNumber() const;

  void setEndpointReference(const std::string& epr);
  const std::string& getEndpointReference() const;

private:
  // ThisModel
  std::string manufacturer_;
  std::string manufacturerUrl_;
  std::string modelName_;
  std::string presentationUrl_;

  // ThisDevice
  std::string friendlyName_;
  std::string firmwareVersion_;
  std::string serialNumber_;

  // Host
  std::string endpointReference_;
};
