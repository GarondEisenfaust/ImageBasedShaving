#include "ibspch.h"
#include "NaiveReconstruction.h"

NaiveReconstruction::NaiveReconstruction(const std::string& path) : ReconstructionAlgorithm(path)
{
}

cv::Mat_<double> NaiveReconstruction::Reconstruct(const cv::Mat_<double>& image) const
{
  static cv::Mat_<double> regularisation = 0 * cv::Mat::eye(v.cols, v.cols, CV_64F);
  return v * ((v.t() * v + regularisation).inv(cv::DECOMP_CHOLESKY) * (v.t() * image));
}
