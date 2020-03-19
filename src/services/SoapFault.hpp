#pragma once

#include "datamodel/MessageModel.hpp"
#include <exception>

class SoapFault : public std::exception
{
public:
  MESSAGEMODEL::Envelope envelope() const
  {
    // TODO
    return MESSAGEMODEL::Envelope();
  }
};
