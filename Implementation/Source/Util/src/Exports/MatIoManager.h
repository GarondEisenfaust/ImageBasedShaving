#pragma once

class MatIoManager
{
public:
  MatIoManager(MatIoManager const&) = delete;
  void operator=(MatIoManager const&) = delete;
  MatIoManager(MatIoManager&&) = delete;
  MatIoManager& operator =(MatIoManager&&) = delete;
  ~MatIoManager() = default;
  static MatIoManager& Instance();
  cv::Mat& GetOrReadMatrixFromYmlFile(const std::string& fileName);
  cv::Mat& GetOrReadMatrixFromImage(const std::string& imageName);

private:
  MatIoManager() = default;
  std::unordered_map<std::string, std::unique_ptr<cv::Mat>> matricies;
};
