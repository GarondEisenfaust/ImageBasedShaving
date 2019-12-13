#include "ibspch.h"
#include "Application.h"
#include "MatrixUtil.h"
#include "General.h"

namespace fs = std::filesystem;

Application::Application(int argc, const char* const argv[])
{
  const char* keys = "{help h usage ?     |          |         }"
    "{mode m             |          | specifies if the matrix is created from images or .yml files}"
    "{directory d        |.         | specifies in which directory the files, the matrix is create from are}";

  const cv::CommandLineParser commandLineParser(argc, argv, keys);

  helpMode = commandLineParser.has("help");

  if (helpMode)
  {
    commandLineParser.printMessage();
  }
  else
  {
    directory = commandLineParser.get<std::string>("directory");

    const std::string mode = commandLineParser.get<std::string>("mode");
    if (mode == std::string("image"))
    {
      IsRelevantFile = Util::IsImage;
      ReadFile = ReadMatFromImageAndConvertToDouble;
    }
    else if (mode == std::string("yml"))
    {
      IsRelevantFile = Util::IsYml;
      ReadFile = ReadMatFromYml;
    }
  }
}

int Application::Run() const
{
  if (helpMode)
  {
    return 0;
  }

  if (!fs::is_directory(directory))
  {
    std::cout << "Given path is not a directory\n";
    return 1;
  }

  cv::Mat_<double> outputMat;
  for (const auto& fileEntry : fs::directory_iterator(directory))
  {
    fs::path path = fileEntry.path();
    if (IsRelevantFile(path))
    {
      cv::Mat_<double> column = ReadFile(path);
      outputMat.push_back(column.t());
    }
  }

  outputMat = outputMat.t();
  cv::FileStorage fs("matrix.yml", cv::FileStorage::WRITE);
  fs << "NoBeardMat" << outputMat;
  fs.release();
  return 0;
}

cv::Mat_<double> Application::ReadMatFromImageAndConvertToDouble(const fs::path& imagePath)
{
  cv::Mat image = imread(imagePath.string(), cv::IMREAD_ANYCOLOR);
  cv::Mat_<cv::Vec3d> imageDouble;
  image.convertTo(imageDouble, CV_64FC3);
  cv::Mat_<double> column = MatUtil::VectorizeImage<double, cv::Vec3d>(imageDouble);
  return column;
}

cv::Mat_<double> Application::ReadMatFromYml(const std::filesystem::path& ymlPath)
{
  cv::Mat_<double> column;
  cv::FileStorage fs(ymlPath.string(), cv::FileStorage::READ);
  fs["Beard"] >> column;
  return column;
}
