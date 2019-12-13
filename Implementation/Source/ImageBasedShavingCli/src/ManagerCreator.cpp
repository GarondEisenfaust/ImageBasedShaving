#include "ibspch.h"
#include "ManagerCreator.h"
#include "ManagerFactory.h"


ManagerCreator::ManagerCreator(const std::string& key)
{
  ManagerFactory::Instance().Register(key, this);
}
