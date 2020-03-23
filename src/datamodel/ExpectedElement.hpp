#pragma once

#include <exception>
#include <string>

/// @brief ExpectedElement models an error while parsing an xml message and expecting an node or
/// attribute at the current position
class ExpectedElement : public std::exception
{
public:
  /// @brief constructs a new ExpectedElement exception
  /// @param name the name of the element which was expected
  /// @param ns the namespace the expected element is from
  ExpectedElement(std::string name, std::string ns);

  /// @brief returns the name of the expected element
  /// @return the name of the element
  std::string name() const
  {
    return name_;
  }

  /// @brief returns the xml namespace of the expected element
  /// @return the namespace of the element
  std::string ns() const
  {
    return ns_;
  }

  const char* what() const noexcept override;

private:
  /// the name of the expected element
  const std::string name_;
  /// the xml namespace of the expected element
  const std::string ns_;
};
