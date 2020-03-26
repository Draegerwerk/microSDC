#pragma once

#include "SoapService.hpp"

class MicroSDC;
class MetadataProvider;

/// @brief GetService implements the SDC Get service
class GetService : public SoapService
{
public:
  /// @brief constructs a new GetService from given metadata
  /// @param microSDC a reference to the MicroSDC instance holding this service
  /// @param metadata a pointer to the metadata describing configurational data
  GetService(const MicroSDC& microSDC, std::shared_ptr<const MetadataProvider> metadata);

  std::string getURI() const override;
  void handleRequest(std::unique_ptr<Request> req) override;

private:
  /// a reference to the microSDC instance holding this service
  const MicroSDC& microSDC_;
  /// a pointer to the metadata
  const std::shared_ptr<const MetadataProvider> metadata_;
};
