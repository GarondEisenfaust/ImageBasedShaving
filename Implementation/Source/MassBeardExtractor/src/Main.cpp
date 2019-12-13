#include "ibspch.h"
#include "MatrixUtil.h"
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
    Log::GetLogger()->error("{}: is not a directory", directory.string().c_str());
    return 1;
  }

  fs::create_directories("./results/original");
  fs::create_directories("./results/beardremoved");
  fs::create_directories("./results/beard");
  fs::create_directories("./results/beardMats");

  for (const auto& filepath : fs::directory_iterator(directory))
  {
    fs::path path(filepath.path());

    if (!Util::IsImage(path))
    {
      Log::GetLogger()->warn("not an image {}", path.string().c_str());
      continue;
    }

    cv::Mat_<cv::Vec3d> image = MatUtil::ReadImageAndConvertTodouble(path.c_str());

    const unsigned int rows = image.rows;
    const unsigned int columns = image.cols;
    const unsigned int channels = image.channels();

    cv::Mat_<double> vectorized = MatUtil::VectorizeImage<double, cv::Vec3d>(image);

    const std::unique_ptr<ReconstructionAlgorithm> reconstructionAlgorithm = ReconstructionFactory::Instance().
      Create("irlsGpu");
    cv::Mat_<double> reconstructed = reconstructionAlgorithm->Reconstruct(vectorized);
    cv::Mat_<cv::Vec3d> beardLess = MatUtil::DeVectorizeImage<double, cv::Vec3d>(
      reconstructed, channels, rows);

    cv::Mat_<cv::Vec3d> beard = image - beardLess;
    cv::Mat_<double> beardVector = MatUtil::VectorizeImage<double, cv::Vec3d>(beard).clone();
    beard = beard / 2 + 127.5;
    std::string fileName = path.stem().string();
    std::string fileEnding = path.extension().string();

    imwrite("./results/original/" + fileName + fileEnding, image);
    imwrite("./results/beardremoved/" + fileName + fileEnding, beardLess);
    imwrite("./results/beard/" + fileName + fileEnding, beard);
    cv::FileStorage fs("./results/beardMats/" + fileName + ".yml", cv::FileStorage::WRITE);
    fs << "Beard" << beardVector;
    fs.release();

    Log::GetLogger()->info("Extracted {}", fileName + fileEnding);
  }
}
