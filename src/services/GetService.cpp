#include "GetService.hpp"

#include "Log.hpp"
#include "MicroSDC.hpp"
#include "WebServer/Request.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "MetadataProvider.hpp"
#include "services/SoapFault.hpp"

static constexpr const char* TAG = "GetService";

GetService::GetService(const MicroSDC& microSDC, std::shared_ptr<const MetadataProvider> metadata)
  : microSDC_(microSDC)
  , metadata_(std::move(metadata))
{
}

std::string GetService::getURI() const
{
  return MetadataProvider::getGetServicePath();
}

void GetService::handleRequest(std::unique_ptr<Request> req)
{
  const auto& requestEnvelope = req->getEnvelope();
  const auto& soapAction = requestEnvelope.header.action;
  if (soapAction == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    metadata_->fillGetServiceMetadata(responseEnvelope);
    responseEnvelope.header.action =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_METADATA_RESPONSE);
    req->respond(responseEnvelope);
  }
  else if (soapAction == SDC::ACTION_GET_MDIB_REQUEST)
  {
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.header.action = WS::ADDRESSING::URIType(SDC::ACTION_GET_MDIB_RESPONSE);
    responseEnvelope.body.getMdibResponse =
        std::make_optional<MESSAGEMODEL::Body::GetMdibResponseType>(microSDC_.getMdib());
    req->respond(responseEnvelope);
  }
  else
  {
    LOG(LogLevel::ERROR, "Unknown soap action " << soapAction);
    req->respond(SoapFault().envelope());
  }
}
