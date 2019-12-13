#include "ibspch.h"
#include "Exports/ReconstructionFactory.h"
#include "ReconstructionAlgorithm.h"
#include "ReconstructionCreatorImpl.h"
#include "NaiveReconstruction.h"
#include "IrlsReconstruction.h"
#include "IrlsReconstructionGpu.h"
#include "LayerReconstruction.h"
#include "LayerReconstructionGpu.h"

REGISTERIMPL(NaiveReconstruction, naive)
REGISTERIMPL(IrlsReconstruction, irls)
REGISTERIMPL(IrlsReconstructionGpu, irlsGpu)
REGISTERIMPL(LayerReconstruction, layer)
REGISTERIMPL(LayerReconstructionGpu, layerGpu)

ReconstructionFactory& ReconstructionFactory::Instance()
{
  static ReconstructionFactory instance;
  return instance;
}

std::unique_ptr<ReconstructionAlgorithm> ReconstructionFactory::Create(const std::string& className)
{
  auto i = creators.find(className);

  if (i != creators.end())
  {
    return std::move(i->second->Create());
  }
  return std::unique_ptr<ReconstructionAlgorithm>(nullptr);
}

void ReconstructionFactory::Register(const std::string& key, ReconstructionCreator* creator)
{
  creators[key] = creator;
}
