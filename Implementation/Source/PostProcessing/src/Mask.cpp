#include "ibspch.h"
#include "Exports/Mask.h"
#include "TransformableFace.h"

namespace PostProcessing
{
  cv::Mat_<cv::Vec3d> ZeroOutUpperPart(cv::Mat_<cv::Vec3d>& image, int cutoff)
  {
    cv::Mat upperFaceMask(cutoff, 95, CV_64FC3, cv::Scalar_<double>(0, 0, 0));
    upperFaceMask.push_back(cv::Mat(95 - cutoff, 95, CV_64FC3, cv::Scalar_<double>(1, 1, 1)));
    image = image.mul(upperFaceMask);
    return image;
  }

  cv::Mat_<cv::Vec3d> ZeroOutExactMask(cv::Mat_<cv::Vec3d>& image)
  {
    cv::Mat_<cv::Vec3d> target;
    image.copyTo(target, TransformableFace().DrawMask());
    return target;
  }

  cv::Mat_<cv::Vec3d> ZeroOutExactMaskNoMouth(cv::Mat_<cv::Vec3d>& image)
  {
    cv::Mat_<cv::Vec3d> target;
    image.copyTo(target, TransformableFace().DrawMaskWithoutMouth());
    return target;
  }
}
