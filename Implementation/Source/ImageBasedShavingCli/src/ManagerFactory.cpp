#include "ibspch.h"
#include "ManagerFactory.h"
#include "ManagerBase.h"
#include "ManagerCreatorImpl.h"

ManagerFactory& ManagerFactory::Instance()
{
  static ManagerFactory instance;
  return instance;
}

std::unique_ptr<ManagerBase> ManagerFactory::Create(const std::string& className)
{
  auto i = creators.find(className);

  if (i != creators.end())
  {
    return std::move(i->second->Create());
  }
  return std::unique_ptr<ManagerBase>(nullptr);
}

void ManagerFactory::Register(const std::string& key, ManagerCreator* creator)
{
  creators[key] = creator;
}
