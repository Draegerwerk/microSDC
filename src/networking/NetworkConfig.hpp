#pragma once

#include <string>

class NetworkConfig
{
public:
  NetworkConfig(bool useTLS, std::string ipAddress);

  /**
   * TODO
   */
  void setUseTLS(bool useTLS);

  /**
   * TODO
   */
  bool useTLS() const;

  /**
   * TODO
   */
  const std::string& ipAddress() const;

  /**
   * TODO
   */
  std::string& ipAddress();

private:
  /// whether to use TLS encrypted communication
  bool useTLS_{true};
  /// the ip address to use
  std::string ipAddress_;
};
