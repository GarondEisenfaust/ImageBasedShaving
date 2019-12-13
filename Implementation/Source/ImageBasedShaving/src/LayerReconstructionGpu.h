#pragma once
#include "LayerReconstruction.h"

class LayerReconstructionGpu : public LayerReconstruction
{
public:
  LayerReconstructionGpu(const std::string& faceMatrix = "Resources/matricies/faces.yml",
                                const std::string& beardMatrix = "Resources/matricies/beardPcs.yml",
                                const std::string& meanBeardMatrix = "Resources/matricies/meanBeard.yml");
  ~LayerReconstructionGpu() = default;
  LayerReconstructionGpu(LayerReconstructionGpu&& other) noexcept = default;
  LayerReconstructionGpu(const LayerReconstructionGpu& other) = default;
  LayerReconstructionGpu& operator=(LayerReconstructionGpu const& other) = default;
  LayerReconstructionGpu& operator=(LayerReconstructionGpu&& other) noexcept = default;

  cv::Mat_<double> ReconstructionIterate(const cv::Mat_<double>& image) const override;
};
