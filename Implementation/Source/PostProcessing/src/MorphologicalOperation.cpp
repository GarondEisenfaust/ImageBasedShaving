#include "ibspch.h"
#include "Exports/MorphologicalOperation.h"

namespace PostProcessing
{
  cv::Mat_<cv::Vec3d> Erosion(cv::Mat_<cv::Vec3d>& image, int kernelSize)
  {
    erode(image, image, cv::Mat::ones(kernelSize, kernelSize, CV_64F));
    return image;
  }

  cv::Mat_<cv::Vec3d> Dilation(cv::Mat_<cv::Vec3d>& image, int kernelSize)
  {
    dilate(image, image, cv::Mat::ones(kernelSize, kernelSize, CV_64F));
    return image;
  }

  cv::Mat_<cv::Vec3d> Opening(cv::Mat_<cv::Vec3d>& image, int kernelSize)
  {
    morphologyEx(image, image, cv::MORPH_OPEN, cv::Mat::ones(kernelSize, kernelSize, CV_64F));
    return image;
  }

  cv::Mat_<cv::Vec3d> Closing(cv::Mat_<cv::Vec3d>& image, int kernelSize)
  {
    morphologyEx(image, image, cv::MORPH_CLOSE, cv::Mat::ones(kernelSize, kernelSize, CV_64F));
    return image;
  }
}
