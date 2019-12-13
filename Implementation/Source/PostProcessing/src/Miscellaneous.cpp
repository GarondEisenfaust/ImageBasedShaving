#include "ibspch.h"
#include "Exports/Miscellaneous.h"

namespace PostProcessing
{
  struct ThresholdingOperator
  {
    double threshold;

    void operator ()(cv::Vec3d& pixel, const int* position) const
    {
      double sum = abs(pixel[0]) + abs(pixel[1]) + abs(pixel[2]);
      if (sum < threshold)
      {
        pixel[0] = 0.0;
        pixel[1] = 0.0;
        pixel[2] = 0.0;
      }
    }
  };

  cv::Mat_<cv::Vec3d> GrabCut(cv::Mat_<cv::Vec3d>& image)
  {
    cv::Mat_<uchar> mask;
    cv::Rect foreground(0, 36, 95, 60);
    cv::Mat_<cv::Vec3b> imageU;

    cv::Mat bgdModel(1, 65, CV_64FC1, cv::Scalar(0));
    cv::Mat fgdModel(1, 65, CV_64FC1, cv::Scalar(0));

    cv::Mat_<cv::Vec3d> imageAbs = abs(image);

    imageAbs.convertTo(imageU, CV_8UC3);
    grabCut(imageU, mask, foreground, bgdModel, fgdModel, 4, cv::GC_INIT_WITH_RECT);
    cv::Mat_<uchar> foregroundMask = (mask == cv::GC_FGD) | (mask == cv::GC_PR_FGD);
    cv::Mat_<cv::Vec3d> beard;
    image.copyTo(beard, foregroundMask);
    return beard;
  }

  cv::Mat_<cv::Vec3d> Threshold(cv::Mat_<cv::Vec3d>& image, double threshold)
  {
    image.forEach(ThresholdingOperator{threshold});
    return image;
  }

  cv::Mat_<cv::Vec3d> AdaptiveThreshold(cv::Mat_<cv::Vec3d>& image, double part)
  {
    cv::Mat_<double> rgbSumImage;
    rgbSumImage.reserve(image.rows);
    for (const auto& pixel : image)
    {
      rgbSumImage.push_back(abs(pixel[0]) + abs(pixel[1]) + abs(pixel[2]));
    }
    const double thresh = CalculateAdaptiveThreshold(rgbSumImage, part);
    image.forEach(ThresholdingOperator{thresh});
    return image;
  }

  double CalculateAdaptiveThreshold(const cv::Mat_<double>& image, double part)
  {
    static unsigned int offset = static_cast<unsigned int>(part * image.rows / 10);
    std::vector<double> vectorFromMat;
    image.copyTo(vectorFromMat);
    std::nth_element(vectorFromMat.begin(), vectorFromMat.begin() + offset, vectorFromMat.end());
    return vectorFromMat[offset];
  }
}
