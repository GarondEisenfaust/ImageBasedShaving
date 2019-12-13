#pragma once

namespace PostProcessing
{
  cv::Mat_<cv::Vec3d> AdaptiveThreshold(cv::Mat_<cv::Vec3d>& image, double part = 8);
  cv::Mat_<cv::Vec3d> GrabCut(cv::Mat_<cv::Vec3d>& image);
  cv::Mat_<cv::Vec3d> Threshold(cv::Mat_<cv::Vec3d>& image, double threshold = 75);
  double CalculateAdaptiveThreshold(const cv::Mat_<double>& image, double part = 8);
}
