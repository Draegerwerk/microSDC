#pragma once

#include "DeviceCharacteristics.hpp"
#include "SubscriptionManager.hpp"
#include "WebServer.hpp"
#include "datamodel/BICEPS_ParticipantModel.hpp"
#include "dpws/DPWSHost.hpp"
#include <map>
#include <mutex>
#include <thread>

class StateHandler;

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
  /**
   * @brief gets the mdib representation of this MicroSDC instance
   * @return constant reference to the mdib
   */
  const BICEPS::PM::Mdib& getMdib() const;
  /**
   * @brief updates the MdDescription part of the mdib
   * @param mdDescription the new mdDescription
   */
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
  /**
   * @brief Adds an state and its handle
   * @param stateHandler the pointer the stateHandler to add
   */
  void addMdState(std::shared_ptr<StateHandler> stateHandler);
  /**
   * @brief updates a given state in the mdib representation
   * @param state the state to update
   */
  void updateState(const std::shared_ptr<BICEPS::PM::NumericMetricState>& state);

  void setLocation(const std::string& descriptorHandle,
                   const BICEPS::PM::LocationDetailType& locationDetail);

private:
  std::shared_ptr<BICEPS::PM::LocationContextState> locationContextState_{nullptr};
  /// the SDC thread
  std::thread sdcThread_;
  /// pointer to the DPWS service
  std::unique_ptr<DPWSHost> dpws_{nullptr};
  /// pointer to the subscription manager
  std::shared_ptr<SubscriptionManager> subscriptionManager_{nullptr};
  /// pointer to the WebServer
  std::unique_ptr<WebServer> webserver_{nullptr};
  /// pointer to the mdib representation
  std::unique_ptr<BICEPS::PM::Mdib> mdib_{nullptr};
  /// mutex protecting changes in the mdib
  mutable std::mutex mdibMutex_;
  /// all states
  std::vector<std::shared_ptr<StateHandler>> stateHandlers_;
  /// whether the communication uses TLS
  bool useTLS_{true};
  /// whether SDC is started and connected
  bool running_{false};
  /// mutex protecting running_ member
  mutable std::mutex runningMutex_;
  /// endpoint reference of this MicroSDC instance
  std::string endpointReference_;
  /// mutex protecting the endpointReference
  mutable std::mutex eprMutex_;

  /// Device Characteristics of this instance
  DeviceCharacteristics deviceCharacteristics_;
  /// mutex protecting the deviceCharacteristics
  mutable std::mutex deviceCharacteristicsMutex_;

  /**
   * @brief Starts and initializes all SDC components and services
   */
  void startup();
  /**
   * @brief updates the internal mdib representation with a given state
   * @tparam infered state type of the state to update
   * @param state the new state to update in the mdib
   */
  template <class T>
  std::shared_ptr<const T> updateMdib(std::shared_ptr<T> state);
  /**
   * @brief update the version attribute of the mdib. Will be called everytime the mdib changes
   */
  void incrementMdibVersion();
  /**
   * @brief returns the current mdib Version
   * @return the mdib version
   */
  unsigned int getMdibVersion() const;
  /**
   * @brief initializes all registered states by calling there initial state function
   */
  void initializeMdStates();
  void notifyEpisodicMetricReport(std::shared_ptr<const BICEPS::PM::NumericMetricState> state);
};
