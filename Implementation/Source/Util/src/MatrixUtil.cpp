#include "ibspch.h"
#include "Exports/MatrixUtil.h"
#include "Exports/General.h"

namespace MatUtil
{
  void WriteVectorizedImage(const std::string& name, const cv::Mat_<double>& vectorizedImage, int channels,
                            int rows)
  {
    const cv::Mat_<cv::Vec3d> outputColored = DeVectorizeImage<double, cv::Vec3d>(vectorizedImage, channels, rows);
    WriteImage(name, outputColored);
  }

  void WriteImage(const std::string& name, const cv::Mat_<cv::Vec3d>& image)
  {
    cv::Mat_<cv::Vec3b> outWrite;
    Util::CreateDirectoryIfAbsent(name);
    imwrite(name, image);
  }

  cv::Mat_<double> ReadMatrixFromFile(const fs::path& path)
  {
    cv::Mat_<cv::Vec3f> v;
    cv::FileStorage storage(path.string(), cv::FileStorage::READ);
    storage["NoBeardMat"] >> v;
    storage.release();
    cv::Mat_<double> vD;
    v.convertTo(vD, CV_64F);
    return vD;
  }

  cv::Mat_<cv::Vec3d> ReadImageAndConvertTodouble(const fs::path& imageName)
  {
    cv::Mat image = imread(imageName.string(), cv::IMREAD_ANYCOLOR);
    cv::Mat_<cv::Vec3d> imageDouble;
    image.convertTo(imageDouble, CV_64FC3);
    return imageDouble;
  }
}
