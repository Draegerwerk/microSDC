#include "ExpectedElement.hpp"

ExpectedElement::~ExpectedElement() throw() {}

ExpectedElement::ExpectedElement(const std::string& name, const std::string& ns)
  : name_(name)
  , namespace__(ns)
{
}

const char* ExpectedElement::what() const noexcept
{
  return "expected element not encountered";
}
