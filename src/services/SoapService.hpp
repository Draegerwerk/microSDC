#pragma once

#include "ServiceInterface.hpp"
#include "datamodel/MessageModel.hpp"
#include <exception>

/// @brief SoapService defines an interface to a very general SOAP service
class SoapService : public ServiceInterface
{
public:
  /// @brief fills the given envelope with reply header information from a given request
  /// @param[out] envelope the envelope of the header to fill
  /// @param request the request holding information of the reply data
  static void fillResponseMessageFromRequestMessage(MESSAGEMODEL::Envelope& envelope,
                                                    const MESSAGEMODEL::Envelope& request);
};
