#pragma once

#include "ServiceInterface.hpp"
#include "datamodel/MessageModel.hpp"
#include <exception>

// TODO: add doxygen
class SoapService : public ServiceInterface
{
public:
  static void fillResponseMessageFromRequestMessage(MESSAGEMODEL::Envelope& envelope,
                                                    const MESSAGEMODEL::Envelope& request);
};
