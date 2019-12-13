#include "ibspch.h"
#include "ManagerBase.h"

int ManagerBase::Manage(const Props& props) const
{
  return 0;
}

ManagerBase::Props::Props(const std::string& mode, const std::string& source, const std::string& destination,
                          const std::string& beardedFace,
                          const std::string& algorithm, const bool extract, const bool extractBeard,
                          const unsigned int index) : mode(mode), source(source), destination(destination),
                                                      beardedFace(beardedFace),
                                                      algorithm(algorithm), extract(extract),
                                                      extractBeard(extractBeard), index(index)
{
}
