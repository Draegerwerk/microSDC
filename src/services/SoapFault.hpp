#pragma once

#include "datamodel/MessageModel.hpp"
#include <exception>

// TODO: add doxygen
class SoapFault : public std::exception
{
public:
  MESSAGEMODEL::Envelope envelope() const
  {
    // TODO: do something useful here. Fill the envelope with an actual error
    return MESSAGEMODEL::Envelope();
  }
};
