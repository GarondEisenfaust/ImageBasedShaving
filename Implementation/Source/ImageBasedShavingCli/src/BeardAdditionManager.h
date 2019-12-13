#pragma once
#include "ManagerBase.h"

class BeardAdditionManager : public ManagerBase
{
public:
  BeardAdditionManager();
  ~BeardAdditionManager() = default;
  BeardAdditionManager(BeardAdditionManager&& other) noexcept = default;
  BeardAdditionManager(const BeardAdditionManager& other) = default;
  BeardAdditionManager& operator=(BeardAdditionManager const& other) = default;
  BeardAdditionManager& operator=(BeardAdditionManager&& other) = default;
  int Manage(const Props& props) const override;
};
