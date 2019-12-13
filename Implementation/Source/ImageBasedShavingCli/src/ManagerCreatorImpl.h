#pragma once
#include <memory>
#include "ManagerBase.h"
#include "ManagerCreator.h"

template <class T>
class ManagerCreatorImpl : public ManagerCreator
{
public:
  ManagerCreatorImpl(const std::string& key) : ManagerCreator(key)
  {
  }

  std::unique_ptr<ManagerBase> Create() override;
  ~ManagerCreatorImpl() = default;
};

template <class T>
std::unique_ptr<ManagerBase> ManagerCreatorImpl<T>::Create()
{
  return std::move(std::make_unique<T>());
}
