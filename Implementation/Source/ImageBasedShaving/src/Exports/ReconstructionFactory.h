#pragma once
#include "../ReconstructionCreator.h"

class ReconstructionFactory
{
public:
  static ReconstructionFactory& Instance();
  std::unique_ptr<ReconstructionAlgorithm> Create(const std::string& className);
  void Register(const std::string& className, ReconstructionCreator* creator);
  ~ReconstructionFactory() = default;
  ReconstructionFactory(ReconstructionFactory const&) = delete;
  void operator=(ReconstructionFactory const&) = delete;

private:
  std::map<std::string, ReconstructionCreator*> creators;
  ReconstructionFactory() = default;
};
