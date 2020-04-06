#include "ExpectedElement.hpp"

ExpectedElement::ExpectedElement(std::string name, std::string ns)
  : name_(std::move(name))
  , ns_(std::move(ns))
{
}

const char* ExpectedElement::what() const noexcept
{
  return "expected element not encountered";
}
