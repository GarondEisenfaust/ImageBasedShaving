#pragma once
#include "IrlsReconstruction.h"

class IrlsReconstructionGpu : public IrlsReconstruction
{
public:
  IrlsReconstructionGpu(const std::string& path = "Resources/matricies/faces.yml");
  ~IrlsReconstructionGpu() = default;
  IrlsReconstructionGpu(IrlsReconstructionGpu&& other) noexcept = default;
  IrlsReconstructionGpu(const IrlsReconstructionGpu& other) = default;
  IrlsReconstructionGpu& operator=(IrlsReconstructionGpu const& other) = default;
  IrlsReconstructionGpu& operator=(IrlsReconstructionGpu&& other) noexcept = default;

  cv::Mat_<double> Reconstruct(const cv::Mat_<double>& image) const override;

private:
  static cv::Mat_<double> CalculateReconstructionCoefficientsGpu(const cv::Mat_<double>& v, const cv::Mat_<double>& w,
                                                                 const cv::Mat_<double>& image);
  static cv::UMat calculateTransposed(const cv::UMat& vGpu);
};
