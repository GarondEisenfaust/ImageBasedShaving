#include "ibspch.h"
#include "Exports/BeardAdder.h"
#include "MatIoManager.h"
#include "MatrixUtil.h"

BeardAdder::BeardAdder(const std::string& meanBeardMatrix, const std::string& beardMat)
{
  beards = MatIoManager::Instance().GetOrReadMatrixFromYmlFile(meanBeardMatrix);
  meanBeard = MatIoManager::Instance().GetOrReadMatrixFromYmlFile(beardMat);
}

BeardAdder& BeardAdder::Instance()
{
  static BeardAdder instance;
  return instance;
}

cv::Mat_<cv::Vec3d> BeardAdder::Add(const cv::Mat_<cv::Vec3d>& image, int index) const
{
  const cv::Mat_<cv::Vec3d> beard = 1000 * MatUtil::DeVectorizeImage<cv::Vec3d, double>(
    beards.col(index).clone(), 3, 95);
  const cv::Mat_<cv::Vec3d> meanBeardImage = MatUtil::DeVectorizeImage<cv::Vec3d, double>(
    meanBeard, 3, 95);
  return image + meanBeardImage + beard;
}

cv::Mat_<double> BeardAdder::Add(const cv::Mat_<double>& image, int index) const
{
  return image + meanBeard + 1000 * beards.col(index);
}

cv::Mat_<cv::Vec3d> BeardAdder::Add(const cv::Mat_<cv::Vec3d>& image, const cv::Mat_<cv::Vec3d>& beard) const
{
  return image + beard;
}

cv::Mat_<double> BeardAdder::Add(const cv::Mat_<double>& image, const cv::Mat_<double>& beard) const
{
  return image + beard;
}

std::vector<cv::Mat_<double>> BeardAdder::ProjectPC(int firstN) const
{
  const auto faces = MatIoManager::Instance().GetOrReadMatrixFromYmlFile("matrix.yml");
  cv::Mat_<double> meanFace;
  reduce(faces, meanFace, 1, cv::REDUCE_SUM, CV_64F);
  meanFace /= faces.cols;

  std::vector<cv::Mat_<double>> pcs;
  for (int i = 0; i < firstN; i++)
  {
    pcs.emplace_back(Add(meanFace, i));
  }
  return pcs;
}

std::vector<cv::Mat_<cv::Vec3d>> BeardAdder::GetFirstPcs(int firstN) const
{
  std::vector<cv::Mat_<cv::Vec3d>> pcs;
  for (int i = 0; i < firstN; i++)
  {
    pcs.push_back(MatUtil::DeVectorizeImage<cv::Vec3d, double>(beards.col(i).clone(), 3, 95));
  }
  return pcs;
}

cv::Mat_<double> BeardAdder::DistanceTransform(const cv::Mat_<cv::Vec3d>& image) const
{
  cv::Mat_<cv::Vec3b> charImage;
  const cv::Mat_<cv::Vec3d> absImage = abs(image);
  absImage.convertTo(charImage, CV_8UC3);
  cv::Mat_<uchar> greyImage(image.rows, image.cols);

  cv::cvtColor(charImage, greyImage, cv::COLOR_BGR2GRAY);

  cv::Mat_<float> distance(image.rows, image.cols);

  cv::distanceTransform(greyImage, distance, cv::DIST_L1, cv::DIST_MASK_5);
  cv::Mat_<double> doubleDistance;
  distance.convertTo(doubleDistance, CV_64F);
  return 1 / (doubleDistance + 0.00001);
}
