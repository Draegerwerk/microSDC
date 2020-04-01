#include "ExpectedElement.hpp"

ExpectedElement::ExpectedElement(std::string name, std::string ns)
  : name_(std::move(name))
  , ns_(std::move(ns))
{
}

const char* ExpectedElement::what() const noexcept
{
  auto msg = "expected element " + name_ + " from namespace " + ns_ + " not encountered";
  return msg.c_str();
}
