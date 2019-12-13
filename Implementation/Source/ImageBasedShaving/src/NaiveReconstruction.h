#pragma once
#include "ReconstructionAlgorithm.h"

class NaiveReconstruction : public ReconstructionAlgorithm
{
public:
  NaiveReconstruction(const std::string& path = "Resources/matricies/faces.yml");
  ~NaiveReconstruction() = default;
  NaiveReconstruction(NaiveReconstruction&& other) noexcept = default;
  NaiveReconstruction(const NaiveReconstruction& other) = default;
  NaiveReconstruction& operator=(NaiveReconstruction const& other) = default;
  NaiveReconstruction& operator=(NaiveReconstruction&& other) noexcept = default;

  cv::Mat_<double> Reconstruct(const cv::Mat_<double>& image) const override;
};
