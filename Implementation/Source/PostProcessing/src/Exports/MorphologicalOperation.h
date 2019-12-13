#pragma once

namespace PostProcessing
{
  cv::Mat_<cv::Vec3d> Erosion(cv::Mat_<cv::Vec3d>& image, int kernelSize = 3);
  cv::Mat_<cv::Vec3d> Dilation(cv::Mat_<cv::Vec3d>& image, int kernelSize = 3);
  cv::Mat_<cv::Vec3d> Opening(cv::Mat_<cv::Vec3d>& image, int kernelSize = 3);
  cv::Mat_<cv::Vec3d> Closing(cv::Mat_<cv::Vec3d>& image, int kernelSize = 3);
};
