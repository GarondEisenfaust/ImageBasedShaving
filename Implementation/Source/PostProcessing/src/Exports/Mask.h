#pragma once
#include <opencv2/core/matx.hpp>

namespace PostProcessing
{
  cv::Mat_<cv::Vec3d> ZeroOutUpperPart(cv::Mat_<cv::Vec3d>& image, int cutoff = 35);
  cv::Mat_<cv::Vec3d> ZeroOutExactMask(cv::Mat_<cv::Vec3d>& image);
  cv::Mat_<cv::Vec3d> ZeroOutExactMaskNoMouth(cv::Mat_<cv::Vec3d>& image);
}
