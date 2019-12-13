#include "ibspch.h"
#include "LayerReconstructionGpu.h"
#include "MatrixUtil.h"

LayerReconstructionGpu::
LayerReconstructionGpu(const std::string& faceMatrix, const std::string& beardMatrix,
                              const std::string& meanBeardMatrix) :
  LayerReconstruction(faceMatrix, beardMatrix, meanBeardMatrix)
{
}

cv::Mat_<double> LayerReconstructionGpu::ReconstructionIterate(const cv::Mat_<double>& image) const
{
  cv::Mat_<double> combined;
  hconcat(v, beards, combined);
  static const cv::UMat regularisation = cv::UMat::eye(combined.cols, combined.cols, CV_64F);
  cv::UMat combinedGpu;
  combinedGpu = combined.getUMat(cv::ACCESS_READ);
  cv::UMat left;
  gemm(combinedGpu, combinedGpu, 1.0, regularisation, 0.0, left, cv::GEMM_1_T);
  cv::Mat_<double> coefficients = left.getMat(cv::ACCESS_READ).inv(cv::DECOMP_CHOLESKY) * (combined.t() * (image -
    meanBeard));

  const cv::Rect faceCoefficientsRect(0, 0, 1, v.cols);
  const cv::Mat_<double> faceVector = v * coefficients(faceCoefficientsRect);

  const cv::Rect beardCoefficientsRect(0, v.cols, 1, beards.cols);
  cv::Mat_<double> beardCoefficients = coefficients(beardCoefficientsRect);
  const cv::Mat_<double> beardVector = beards * beardCoefficients + meanBeard;

  const cv::Mat_<double> otherLayerVector = image - (faceVector + beardVector);

  const cv::Mat_<double> output = faceVector + otherLayerVector;

  DebugOutput(output, faceVector, beardVector, otherLayerVector);
  PLOTCOEFFICIENTS(beardCoefficients);

  return output;
}
