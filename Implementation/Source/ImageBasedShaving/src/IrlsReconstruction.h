#pragma once
#include "ReconstructionAlgorithm.h"

class IrlsReconstruction : public ReconstructionAlgorithm
{
public:
  IrlsReconstruction(const std::string& path = "Resources/matricies/faces.yml");
  ~IrlsReconstruction() = default;
  IrlsReconstruction(IrlsReconstruction&& other) noexcept = default;
  IrlsReconstruction(const IrlsReconstruction& other) = default;
  IrlsReconstruction& operator=(IrlsReconstruction const& other) = default;
  IrlsReconstruction& operator=(IrlsReconstruction&& other) noexcept = default;

  cv::Mat_<double> Reconstruct(const cv::Mat_<double>& image) const override;

protected:
  cv::Mat_<double> Reconstruct(const cv::Mat_<double>& image,
                               const std::function<cv::Mat_<double>(const cv::Mat_<double>&, const cv::Mat_<double>&,
                                                                    const cv::Mat_<double>&)>&
                               calculateReconstructionCoefficients) const;
  static cv::Mat_<double> WeightMatrix(const cv::Mat_<double>& weights, const cv::Mat_<double>& toWeight);
  static cv::Mat_<float> WeightMatrix(const cv::Mat_<float>& weights, const cv::Mat_<float>& toWeight);

private:
  cv::Mat_<double> CalculateInitialWMeanFace(const cv::Mat_<double>& image) const;
  cv::Mat_<double> CalculateInitialWNearestNeighbour(const cv::Mat_<double>& image) const;
  static cv::Mat_<double> CalculateInitialWZeroOutLowerPart();

  static cv::Mat_<double> CalculateReconstructionCoefficients(const cv::Mat_<double>& v, const cv::Mat_<double>& w,
                                                              const cv::Mat_<double>& image);
  static double CalculateSigmaSquared(const cv::Mat_<double>& residual);
  static double CalculateQuantile(const cv::Mat_<double>& image, double = 0.5);
  static double CalculateMedianHist(const cv::Mat_<double>& image);
  static cv::Mat_<double> CalculateW(cv::Mat_<double>& residual, double sigmaSquared);
  static void DebugOutput(const cv::Mat_<double>& reconstructed, const cv::Mat_<double>& cNew,
                          const cv::Mat_<double>& w, unsigned int i);
  static cv::Mat_<uchar> CreateVectorMask();
};
