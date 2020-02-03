#pragma once

#include <exception>
#include <string>

class ExpectedElement : public std::exception
{
public:
  ~ExpectedElement() noexcept override;
  ExpectedElement(const std::string& name, const std::string& ns);

  std::string name() const
  {
    return name_;
  }

  std::string namespace_() const
  {
    return namespace__;
  }

  const char* what() const noexcept override;

private:
  const std::string name_;
  const std::string namespace__;
};
