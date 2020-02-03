#pragma once

#include <memory>
#include <optional>
#include <string>

namespace WS::EVENTING
{
  class Identifier : public std::string
  {
  public:
    using IsReferenceParameterType = bool;
    using IsReferenceParameterOptional = std::optional<IsReferenceParameterType>;

  protected:
    IsReferenceParameterOptional IsReferenceParameter_;
  };
} // namespace WS::EVENTING
