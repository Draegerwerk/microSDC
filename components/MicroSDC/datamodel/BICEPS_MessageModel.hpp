#pragma once

#include "BICEPS_ParticipantModel.hpp"
#include <string>

namespace BICEPS::MM
{
  class GetMdib
  {
  };

  class GetMdibResponse
  {
  public:
    using MdibType = BICEPS::PM::Mdib;
    const MdibType& Mdib() const;
    MdibType& Mdib();

    GetMdibResponse(const MdibType& mdib);
  protected:
    MdibType Mdib_;
  };
} // namespace BICEPS
