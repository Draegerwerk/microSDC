#include "Log.hpp"
#include "MicroSDC.hpp"
#include "StateHandler.hpp"
#include "networking/NetworkConfig.hpp"
#include <thread>

static volatile std::atomic_bool keepRunning = true;
static std::condition_variable cvRunning;
static std::mutex runningMutex;

void intHandler(int /*unused*/)
{
  keepRunning = false;
  std::unique_lock<std::mutex> lock(runningMutex);
  cvRunning.notify_all();
}

int main()
{
  Log::setLogLevel(LogLevel::INFO);
  LOG(LogLevel::INFO, "Starting up....");

  auto microSDC = std::make_unique<MicroSDC>();

  microSDC->setNetworkConfig(std::make_unique<NetworkConfig>(true, "192.168.178.97", 8080));

  DeviceCharacteristics deviceCharacteristics;
  deviceCharacteristics.setFriendlyName("MicroSDC on Linux");
  deviceCharacteristics.setManufacturer("Draeger");
  deviceCharacteristics.setModelName("MicroSDC_Device01");
  microSDC->setDeviceCharacteristics(deviceCharacteristics);

  microSDC->setEndpointReference("urn:uuid:MicroSDC-provider-on-linux");

  // Construct MdDescription
  BICEPS::PM::SystemContextDescriptor systemContext("system_context");
  systemContext.PatientContext() = BICEPS::PM::PatientContextDescriptor("patient_context");
  systemContext.LocationContext() = BICEPS::PM::LocationContextDescriptor("location_context");

  BICEPS::PM::MdsDescriptor deviceDescriptor("MedicalDevices");
  deviceDescriptor.SystemContext() = systemContext;

  // Dummy state
  auto state = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "state_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  state->SafetyClassification() = BICEPS::PM::SafetyClassification::MedA;

  BICEPS::PM::ChannelDescriptor deviceChannel("device_channel");
  deviceChannel.Metric().emplace_back(state);
  deviceChannel.SafetyClassification() = BICEPS::PM::SafetyClassification::MedA;

  BICEPS::PM::VmdDescriptor deviceModule("device_vmd");
  deviceModule.Channel().emplace_back(deviceChannel);

  deviceDescriptor.Vmd().emplace_back(deviceModule);

  BICEPS::PM::MdDescription mdDescription;
  mdDescription.Mds().emplace_back(deviceDescriptor);

  microSDC->setMdDescription(mdDescription);

  // set location detail
  BICEPS::PM::LocationDetailType locationDetail;
  locationDetail.PoC() = "PoC-A";
  locationDetail.Room() = "Room-A";
  locationDetail.Bed() = "Bed-A";
  locationDetail.Facility() = "Facility-A";
  locationDetail.Building() = "Building-A";
  locationDetail.Floor() = "Floor-A";
  microSDC->setLocation("location_context", locationDetail);

  auto stateHandler = std::make_shared<NumericStateHandler>("state_handle");
  microSDC->addMdState(stateHandler);

  microSDC->start();

  auto valueThread = std::thread([stateHandler]() {
    double i = 0.0;
    while (keepRunning)
    {
      stateHandler->setValue(i++);
      std::unique_lock<std::mutex> lock(runningMutex);
      cvRunning.wait_for(lock, std::chrono::seconds(1));
    }
  });

  struct sigaction sa
  {
  };
  sa.sa_handler = &intHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGINT, &sa, nullptr);
  sigaction(SIGTERM, &sa, nullptr);

  sigset_t mask;
  sigemptyset(&mask);
  while (keepRunning)
  {
    sigsuspend(&mask);
  }
  valueThread.join();
  return 0;
}
