#include "SubscriptionManager.hpp"
#include "datamodel/ws-addressing.hpp"
#include "SDCConstants.hpp"
#include "UUIDGenerator.hpp"


SubscriptionManager::SubscriptionManager()
{
  id_ = SDC::UUID_SDC_PREFIX + UUIDGenerator().create().toString();
}


WS::EVENTING::SubscribeResponse
SubscriptionManager::dispatch(const WS::EVENTING::Subscribe& subscribeRequest)
{
  WS::EVENTING::ExpirationType expires = subscribeRequest.Expires().value();
  WS::ADDRESSING::EndpointReferenceType subscriptionManagerEPR;
  // TODO: get proper address
  subscriptionManagerEPR.Address() =
      WS::ADDRESSING::URIType("https://1.1.1.1/MicroSDC/Set/SubscriptionManager/" + id_);
  subscriptionManagerEPR.ReferenceParameters() =
      std::make_optional<WS::ADDRESSING::ReferenceParametersType>(id_);
  WS::EVENTING::SubscribeResponse subscribeResponse(subscriptionManagerEPR, expires);
  return subscribeResponse;
}
