#pragma once
class Application
{
  // Attributes 
private:
  bool helpMode;
  std::string directory;
  std::function<bool(const std::filesystem::path& path)> IsRelevantFile;
  std::function<cv::Mat_<double>(const std::filesystem::path& imagePath)> ReadFile;

  // Functions
public:
  Application(int argc, const char* const argv[]);
  int Run() const;

private:
  static cv::Mat_<double> ReadMatFromImageAndConvertToDouble(const std::filesystem::path& imagePath);
  static cv::Mat_<double> ReadMatFromYml(const std::filesystem::path& ymlPath);
};
