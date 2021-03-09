#pragma once

#include "DeviceCharacteristics.hpp"
#include "WebServer/WebServer.hpp"
#include "discovery/DiscoveryService.hpp"
#include <map>
#include <mutex>
#include <thread>
#include <vector>

class NetworkConfig;
class StateHandler;
class SubscriptionManager;
namespace BICEPS::PM
{
  struct LocationContextState;
  struct LocationDetail;
  struct MdDescription;
  struct Mdib;
  struct NumericMetricState;
} // namespace BICEPS::PM

/// @brief MicroSDC implements the central SDC instance with an interface to any SDC utility
class MicroSDC
{
public:
  /// @brief constructs an MicroSDC instance
  explicit MicroSDC();

  /// @brief starts the sdcThread calling startup()
  void start();

  /// @brief stops all components when disconnected
  void stop();

  /// @brief returns whether MicroSDC is running
  /// @return whether MicroSDC is running
  bool is_running() const;

  /// @brief gets the mdib representation of this MicroSDC instance
  /// @return constant reference to the mdib
  const BICEPS::PM::Mdib& get_mdib() const;

  /// @brief updates the MdDescription part of the mdib
  /// @param mdDescription the new mdDescription
  void set_md_description(const BICEPS::PM::MdDescription& md_description);

  /// @brief sets the device characteristics of this SDC service
  /// @param devChar characteristics to be set
  void set_device_characteristics(DeviceCharacteristics dev_char);

  /// @brief gets the device characteristics of this SDC service
  /// @return reference to the device characteristics
  const DeviceCharacteristics& get_device_characteristics() const;

  /// @brief set a new endpoint reference of this MicroSDC instance
  /// @param epr the new endpoint reference
  void set_endpoint_reference(const std::string& epr);

  /// @brief gets the endpoint reference of this MicroSDC instance
  /// @return string containing the endpoint reference of this instance
  std::string get_endpoint_reference() const;

  /// @brief sets the network config. This should be set before start is called!
  /// @param networkConfig the pointer to the network configuration
  void set_network_config(std::unique_ptr<NetworkConfig> network_config);

  /// @brief get a valid message id for WS-Addressing
  /// @return string of a message id
  static std::string calculate_message_id();

  /// @brief get a uuid
  /// @return string containing a UUID
  static std::string calculate_uuid();

  /// @brief Adds an state and its handle
  /// @param stateHandler the pointer the stateHandler to add
  void add_md_state(std::shared_ptr<StateHandler> state_handler);

  /// @brief updates a given state in the mdib representation
  /// @param state the state to update
  void update_state(const std::shared_ptr<BICEPS::PM::AbstractState>& state);

  /// @brief sets the location of this instance
  /// @param descriptorHandle the descriptor of the location state descriptor
  /// @param locationDetail the location information to set
  void set_location(const std::string& descriptor_handle,
                    const BICEPS::PM::LocationDetail& location_detail);

  /// @brief request_state_change is called to request a change of state
  /// @param set reference to the base set request
  /// @return invocation state how this action performed
  BICEPS::MM::InvocationState request_state_change(const BICEPS::MM::AbstractSet& set);

private:
  /// a pointer to the location context state holding location descriptor of this instance
  std::shared_ptr<BICEPS::PM::LocationContextState> location_context_state_{nullptr};
  /// the SDC thread
  std::thread sdc_thread_;
  /// pointer to the discovery service
  std::unique_ptr<DiscoveryService> discovery_service_{nullptr};
  /// pointer to the subscription manager
  std::shared_ptr<SubscriptionManager> subscription_manager_{nullptr};
  /// pointer to the WebServer
  std::unique_ptr<WebServerInterface> webserver_{nullptr};
  /// pointer to the mdib representation
  std::unique_ptr<BICEPS::PM::Mdib> mdib_{nullptr};
  /// mutex protecting changes in the mdib
  mutable std::mutex mdib_mutex_;
  /// all states
  std::vector<std::shared_ptr<StateHandler>> state_handlers_;
  /// pointer to the network configuration
  std::shared_ptr<NetworkConfig> network_config_{nullptr};
  /// whether SDC is started or stopped
  bool running_{false};
  /// mutex protecting running_ member
  mutable std::mutex running_mutex_;
  /// endpoint reference of this MicroSDC instance
  std::string endpoint_reference_;
  /// mutex protecting the endpointReference
  mutable std::mutex epr_mutex_;

  /// Device Characteristics of this instance
  DeviceCharacteristics device_characteristics_;


  /// @brief Starts and initializes all SDC components and services
  void startup();

  /// @brief updates the internal mdib representation with a given state
  /// @tparam infered state type of the state to update
  /// @param state the new state to update in the mdib
  std::shared_ptr<const BICEPS::PM::AbstractState>
  update_mdib(std::shared_ptr<BICEPS::PM::AbstractState> state);

  /// @brief update the version attribute of the mdib. Will be called everytime the mdib changes
  void increment_mdib_version();

  /// @brief returns the current mdib Version
  /// @return the mdib version
  unsigned int get_mdib_version() const;

  /// @brief initializes all registered states by calling there initial state function
  void initialize_md_states();

  /// @brief sends a notification to subscriber about a changed numeric metric state
  /// @param state a pointer to the state which was updated
  void notify_episodic_metric_report(std::shared_ptr<const BICEPS::PM::AbstractMetricState> state);


  /// @brief find_operation_target_for_operation_handle searches all sco to find the operation
  /// target that was triggered by the handle
  /// @param handle the handle of the operation to find
  /// @return target of the operation if found
  std::optional<BICEPS::PM::AbstractOperationDescriptor::OperationTargetType>
  find_operation_target_for_operation_handle(
      const BICEPS::PM::AbstractDescriptor::HandleType& handle) const;
};
