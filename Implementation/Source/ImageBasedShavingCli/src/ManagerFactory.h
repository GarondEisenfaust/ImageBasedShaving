#pragma once
#include "ManagerCreator.h"

class ManagerFactory
{
public:
  static ManagerFactory& Instance();
  std::unique_ptr<ManagerBase> Create(const std::string& className);
  void Register(const std::string& className, ManagerCreator* creator);
  ~ManagerFactory() = default;
  ManagerFactory(ManagerFactory const&) = delete;
  void operator=(ManagerFactory const&) = delete;

private:
  std::map<std::string, ManagerCreator*> creators;
  ManagerFactory() = default;
};
