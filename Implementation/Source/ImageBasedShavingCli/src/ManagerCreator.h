#pragma once
#include <string>
#include <memory>
#include "ManagerBase.h"

class ManagerCreator
{
public:
  ManagerCreator(const std::string& className);
  virtual ~ManagerCreator() = default;
  virtual std::unique_ptr<ManagerBase> Create() = 0;
};
