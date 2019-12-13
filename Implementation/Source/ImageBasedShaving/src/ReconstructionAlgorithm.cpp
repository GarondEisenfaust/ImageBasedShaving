#include "ibspch.h"
#include "ReconstructionAlgorithm.h"
#include "MatIoManager.h"

ReconstructionAlgorithm::ReconstructionAlgorithm(const std::string& path) : v(
  MatIoManager::Instance().GetOrReadMatrixFromYmlFile(path))
{
};
