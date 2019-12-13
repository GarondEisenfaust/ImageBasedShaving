#include "ibspch.h"
#include "IrlsReconstructionGpu.h"

IrlsReconstructionGpu::IrlsReconstructionGpu(const std::string& path) : IrlsReconstruction(path)
{
}

cv::Mat_<double> IrlsReconstructionGpu::Reconstruct(const cv::Mat_<double>& image) const
{
  return IrlsReconstruction::Reconstruct(image, CalculateReconstructionCoefficientsGpu);
}

template <class T, int TYPE>
cv::Mat_<T> toTypeMat(cv::Mat mat)
{
  cv::Mat_<T> converted;
  mat.convertTo(converted, TYPE);
  return converted;
}

cv::Mat_<double> IrlsReconstructionGpu::CalculateReconstructionCoefficientsGpu(
  const cv::Mat_<double>& v, const cv::Mat_<double>& w, const cv::Mat_<double>& image)
{
  static const cv::UMat regularisation = cv::UMat::eye(v.cols, v.cols, CV_64F);
  static const cv::UMat vGpu = v.getUMat(cv::ACCESS_READ);
  const cv::Mat_<double> wSquared = w.mul(w);

  cv::UMat leftResult;
  const cv::Mat_<double> vWeighted = WeightMatrix(wSquared, v);
  gemm(vGpu, vWeighted.getUMat(cv::ACCESS_READ), 1.0, regularisation, 0.0, leftResult, cv::GEMM_1_T);

  const cv::Mat_<double> rightResult = v.t() * WeightMatrix(wSquared, image);

  const cv::Mat_<float> resultFloat = leftResult.getMat(cv::ACCESS_READ).inv(cv::DECOMP_CHOLESKY) * rightResult;
  return resultFloat;
}

cv::UMat IrlsReconstructionGpu::calculateTransposed(const cv::UMat& vGpu)
{
  cv::UMat vTGpu;
  transpose(vGpu, vTGpu);
  return vTGpu;
}
