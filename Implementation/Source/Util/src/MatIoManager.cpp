#include "ibspch.h"
#include "Exports/MatIoManager.h"
#include "Exports/MatrixUtil.h"

MatIoManager& MatIoManager::Instance()
{
  static MatIoManager instance;
  return instance;
}

cv::Mat& MatIoManager::GetOrReadMatrixFromYmlFile(const std::string& fileName)
{
  const auto iter = matricies.find(fileName);
  if (iter != matricies.end())
  {
    return *(iter->second);
  }
  auto imagePointer = std::make_unique<cv::Mat_<double>>(MatUtil::ReadMatrixFromFile(fileName));
  matricies.insert(std::make_pair(fileName, std::move(imagePointer)));
  return *matricies[fileName];
}

cv::Mat& MatIoManager::GetOrReadMatrixFromImage(const std::string& imageName)
{
  const auto iter = matricies.find(imageName);
  if (iter != matricies.end())
  {
    return *(iter->second);
  }
  auto imagePointer = std::make_unique<cv::Mat_<cv::Vec3d>>(MatUtil::ReadImageAndConvertTodouble(imageName));
  matricies.insert(std::make_pair(imageName, std::move(imagePointer)));
  return *matricies[imageName];
}
