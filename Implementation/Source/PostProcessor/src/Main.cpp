#include "ibspch.h"
#include "Miscellaneous.h"
#include "Mask.h"
#include "MorphologicalOperation.h"
#include "General.h"

// Original directories
const fs::path beardMatPath(".");

// Directories with processed files
const fs::path outputPath("./output/");

using PostProcessorFunction = std::function<cv::Mat_<cv::Vec3d>(cv::Mat_<cv::Vec3d>&)>;
std::unordered_map<std::string, PostProcessorFunction> InitializePostProcessorMap();

int main(int argc, char* argv[])
{
  fs::create_directories(outputPath);
  const auto postProcessorMap = InitializePostProcessorMap();

  for (const auto& filePath : fs::directory_iterator(beardMatPath))
  {
    if (!Util::IsYml(filePath))
    {
      continue;
    }
    const fs::path beardFilepath = filePath.path();

    cv::Mat_<double> doubleBeardMat;
    cv::FileStorage beardMatStorage(beardFilepath.string(), cv::FileStorage::READ);
    beardMatStorage["Beard"] >> doubleBeardMat;

    cv::Mat_<cv::Vec3d> beardMatDisplay = doubleBeardMat.reshape(3, 95);

    // Postprocessing steps
    for (int i = 1; i < argc; i++)
    {
      const std::string algorithm = argv[i];
      beardMatDisplay = postProcessorMap.at(algorithm)(beardMatDisplay);
    }

    std::string name = beardFilepath.filename().stem().string();
    cv::imwrite(outputPath.string() + name + ".png",
                beardMatDisplay / 2 + 127.5);

    doubleBeardMat = beardMatDisplay.reshape(
      1, beardMatDisplay.rows * beardMatDisplay.cols * beardMatDisplay.channels());

    cv::FileStorage fs(outputPath.string() + beardFilepath.filename().string(), cv::FileStorage::WRITE);
    fs << "Beard" << doubleBeardMat;
    fs.release();
  }
}

std::unordered_map<std::string, PostProcessorFunction> InitializePostProcessorMap()
{
  std::unordered_map<std::string, PostProcessorFunction> functionMap;
  functionMap["AdaptiveThreshold"] = [](cv::Mat_<cv::Vec3d>& image)
  {
    return PostProcessing::AdaptiveThreshold(image);
  };

  functionMap["GrabCut"] = [](cv::Mat_<cv::Vec3d>& image) { return PostProcessing::GrabCut(image); };
  functionMap["Threshold"] = [](cv::Mat_<cv::Vec3d>& image) { return PostProcessing::Threshold(image); };
  functionMap["Erosion"] = [](cv::Mat_<cv::Vec3d>& image) { return PostProcessing::Erosion(image); };
  functionMap["Dilation"] = [](cv::Mat_<cv::Vec3d>& image) { return PostProcessing::Dilation(image); };
  functionMap["Opening"] = [](cv::Mat_<cv::Vec3d>& image) { return PostProcessing::Opening(image); };
  functionMap["Closing"] = [](cv::Mat_<cv::Vec3d>& image) { return PostProcessing::Closing(image); };
  functionMap["ZeroOutUpperPart"] = [](cv::Mat_<cv::Vec3d>& image)
  {
    return PostProcessing::ZeroOutUpperPart(image);
  };
  functionMap["ZeroOutExactMask"] = [](cv::Mat_<cv::Vec3d>& image)
  {
    return PostProcessing::ZeroOutExactMask(image);
  };

  functionMap["ZeroOutExactMaskNoMouth"] = [](cv::Mat_<cv::Vec3d>& image)
  {
    return PostProcessing::ZeroOutExactMaskNoMouth(image);
  };

  return functionMap;
}
