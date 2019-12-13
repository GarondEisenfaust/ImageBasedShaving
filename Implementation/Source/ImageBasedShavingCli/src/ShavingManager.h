#pragma once
#include "ManagerBase.h"

class ShavingManager : public ManagerBase
{
public:
  ShavingManager();
  ~ShavingManager() = default;
  ShavingManager(ShavingManager&& other) noexcept = default;
  ShavingManager(const ShavingManager& other) = default;
  ShavingManager& operator=(ShavingManager const& other) = default;
  ShavingManager& operator=(ShavingManager&& other) = default;
  int Manage(const Props& props) const override;
  cv::Mat_<cv::Vec3d> Shave(const ManagerBase::Props& props, cv::Mat_<cv::Vec3d> faceImage) const;
};
