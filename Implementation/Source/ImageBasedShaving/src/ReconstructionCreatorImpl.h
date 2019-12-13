#pragma once
#include <memory>
#include "ReconstructionAlgorithm.h"
#include "ReconstructionCreator.h"

template <class T>
class ReconstructionCreatorImpl : public ReconstructionCreator
{
public:
  ReconstructionCreatorImpl(const std::string& key) : ReconstructionCreator(key)
  {
  }

  std::unique_ptr<ReconstructionAlgorithm> Create() override;
  ~ReconstructionCreatorImpl() = default;
};

template <class T>
std::unique_ptr<ReconstructionAlgorithm> ReconstructionCreatorImpl<T>::Create()
{
  return std::move(std::make_unique<T>());
}
