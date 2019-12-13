#include "ibspch.h"
#include "General.h"

int main(int argc, char* argv[])
{
  fs::path directory = ".";
  if (argv[1])
  {
    directory = argv[1];
  }

  if (!is_directory(directory))
  {
    Log::GetLogger()->error("{}: is not a directory", directory.string());
    return 1;
  }


  namedWindow("Original", cv::WINDOW_KEEPRATIO);
  namedWindow("BeardRemovalViewer", cv::WINDOW_KEEPRATIO);

  cv::moveWindow("BeardRemovalViewer", 600, 100);
  cv::moveWindow("Original", 300, 100);

  fs::create_directories("./results/noBeard");
  fs::create_directories("./results/beard");
  fs::create_directories("./results/processed");

  for (const auto& filepath : fs::directory_iterator(directory))
  {
    fs::path path(filepath.path());
    if (!Util::IsImage(path))
    {
      continue;
    }

    std::string pathString = path.string();
    cv::Mat image = imread(pathString, cv::IMREAD_ANYCOLOR);

    if (image.data == nullptr)
    {
      Log::GetLogger()->error("Image at {} not found!", pathString);
      continue;
    }
    Log::GetLogger()->info("Image at {} loaded!", pathString);

    auto faces = FaceExtractionManager::Instance().DetectFace(image);

    if (faces.empty())
    {
      Log::GetLogger()->error("No faces were found in {}", pathString);
      continue;
    }

    Log::GetLogger()->info("{} were found", faces.size());

    // Necessary for triangular warping
    image.convertTo(image, CV_64FC3, 1 / 255.0);
    unsigned int faceNum = 0;
    std::string fileName = path.stem().string();
    std::string fileEnding = path.extension().string();

    for (auto& face : faces)
    {
      // OutputImage 
      cv::Mat output = cv::Mat::zeros(95, 95, image.type());

      // Morphs the face
      try
      {
        output = FaceExtractionManager::Instance().MorphToCanonicalFace(image, face);
      }
      catch (const cv::Exception& e)
      {
        Log::GetLogger()->error(e.msg);
        continue;
      }

      output.convertTo(output, CV_8U, 255);
      cv::Mat biggerOutput;
      resize(output, biggerOutput, cv::Size(), 3, 3);
      std::string outputName = fileName + std::to_string(faceNum) + fileEnding;
      imshow("BeardRemovalViewer", biggerOutput);
      cv::setWindowTitle("BeardRemovalViewer", outputName);
      imshow("Original", image);
      cv::setWindowTitle("Original", fileName + fileEnding);
      cv::resizeWindow("Original", 285, 285);
      int keyCode = cv::waitKey();
      // key == 1
      if (keyCode == 49)
      {
        imwrite("./results/noBeard/" + outputName, output);
      }
        // key == 2
      else if (keyCode == 50)
      {
        imwrite("./results/beard/" + outputName, output);
      }

      faceNum++;
    }
    fs::rename(path, "./results/processed/" + fileName + fileEnding);
  }
}
