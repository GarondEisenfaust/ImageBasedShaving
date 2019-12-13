#pragma once
#include  <string>
#include <memory>
#include "ReconstructionAlgorithm.h"

class ReconstructionCreator
{
public:
  ReconstructionCreator(const std::string& className);
  virtual ~ReconstructionCreator() = default;
  virtual std::unique_ptr<ReconstructionAlgorithm> Create() = 0;
};
