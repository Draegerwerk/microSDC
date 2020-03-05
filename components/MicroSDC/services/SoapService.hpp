#pragma once

#include "ServiceInterface.hpp"
#include "datamodel/MessageModel.hpp"
#include "rapidxml.hpp"
#include <exception>

class SoapService : public ServiceInterface
{
public:
  class SoapFault : public std::exception
  {
  };
  static MESSAGEMODEL::Envelope parse(char* message);
  static void fillResponseMessageFromRequestMessage(MESSAGEMODEL::Envelope& envelope,
                                                    const MESSAGEMODEL::Envelope& request);
};
