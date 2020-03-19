#pragma once

#include <exception>
#include <string>

class ExpectedElement : public std::exception
{
public:
  ExpectedElement(std::string name, std::string ns);

  std::string name() const
  {
    return name_;
  }

  std::string ns() const
  {
    return ns_;
  }

  const char* what() const noexcept override;

private:
  const std::string name_;
  const std::string ns_;
};
