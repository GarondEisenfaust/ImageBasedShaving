#include "ibspch.h"
#include "ReconstructionCreator.h"
#include "Exports/ReconstructionFactory.h"

ReconstructionCreator::ReconstructionCreator(const std::string& key)
{
  ReconstructionFactory::Instance().Register(key, this);
}
