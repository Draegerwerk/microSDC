#pragma once

#include "DeviceCharacteristics.hpp"
#include "WebServer.hpp"
#include "dpws/DPWSHost.hpp"
#include <mutex>
#include <thread>

class MicroSDC
{
public:
  /**
   * @brief constructs an MicroSDC instance
   */
  MicroSDC();
  /**
   * @brief starts the sdcThread calling startup()
   */
  void start();
  /**
   * @brief stops all components when disconnected
   */
  void stop();
  const BICEPS::PM::Mdib& getMdib() const;
  void setMdDescription(const BICEPS::PM::MdDescription& mdDescription);
  /**
   * @brief sets the device characteristics of this SDC service
   * @param devChar characteristics to be set
   */
  void setDeviceCharacteristics(DeviceCharacteristics devChar);
  /**
   * @brief gets the device characteristics of this SDC service
   * @return reference to the device characteristics
   */
  const DeviceCharacteristics& getDeviceCharacteristics() const;
  /**
   * @brief set a new endpoint reference of this MicroSDC instance
   * @param epr the new endpoint reference
   */
  void setEndpointReference(const std::string& epr);
  /**
   * @brief gets the endpoint reference of this MicroSDC instance
   * @return string containing the endpoint reference of this instance
   */
  std::string getEndpointReference() const;
  /**
   * @brief sets the useTLS bool member for TLS usage enable. This should be set before start is
   * called!
   * @param useTLS whether to use TLS for web services
   */
  void setUseTLS(bool useTLS);
  /**
   * @brief get a valid message id for WS-Addressing
   * @return string of a message id
   */
  static std::string calculateMessageID();
  /**
   * @brief get a uuid
   * @return string containing a UUID
   */
  static std::string calculateUUID();

private:
  /// the SDC thread
  std::thread sdcThread_;
  /// pointer to the DPWS service
  std::unique_ptr<DPWSHost> dpws_{nullptr};
  /// pointer to the WebServer
  std::unique_ptr<WebServer> webserver_{nullptr};
  /// pointer to the mdib representation
  std::unique_ptr<BICEPS::PM::Mdib> mdib_{nullptr};
  /// whether the communication uses TLS
  bool useTLS_{true};
  /// whether SDC is started and connected
  bool running_{false};
  mutable std::mutex runningMutex_;
  /// endpoint reference of this MicroSDC instance
  std::string endpointReference_;
  mutable std::mutex eprMutex_;

  /// Device Characteristics of this instance
  DeviceCharacteristics deviceCharacteristics_;
  mutable std::mutex deviceCharacteristicsMutex_;

  /**
   * @brief Starts and initializes all SDC components and services
   */
  void startup();
};
