#pragma once

#include "datamodel/MessageModel.hpp"
#include <exception>

/// @brief SoapFault models any failure while processing a SOAP request
class SoapFault : public std::exception
{
public:
  /// @brief constructs an soap envelope holding this soap fault
  /// @return the soap envelop containing the soap fault
  MESSAGEMODEL::Envelope envelope() const
  {
    // TODO: do something useful here. Fill the envelope with an actual error
    return MESSAGEMODEL::Envelope();
  }
};
