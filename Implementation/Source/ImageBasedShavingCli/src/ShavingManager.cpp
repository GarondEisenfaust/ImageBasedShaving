#include "ibspch.h"
#include "ShavingManager.h"
#include "ManagerBase.h"
#include "MatIoManager.h"
#include "FaceExtractionManager.h"
#include "ReconstructionFactory.h"
#include "ManagerCreatorImpl.h"
#include "MatrixUtil.h"

REGISTERMANAGERIMPL(ShavingManager, shaving);

ShavingManager::ShavingManager() : ManagerBase()
{
}

cv::Mat_<cv::Vec3d> ShavingManager::Shave(const ManagerBase::Props& props, cv::Mat_<cv::Vec3d> faceImage) const
{
  const unsigned int rows = faceImage.rows;
  const unsigned int channels = faceImage.channels();

  cv::Mat_<double> vectorized = MatUtil::VectorizeImage<double, cv::Vec3d>(faceImage);

  Log::GetLogger()->info("Running: {}", props.algorithm);
  std::unique_ptr<ReconstructionAlgorithm> reconstructionAlgorithm = ReconstructionFactory::Instance().Create(
    props.algorithm);
  cv::Mat_<double> reconstructed = reconstructionAlgorithm->Reconstruct(vectorized);

  return MatUtil::DeVectorizeImage<double, cv::Vec3d>(
    reconstructed, channels, rows);
}

int ShavingManager::Manage(const Props& props) const
{
  if (props.source.empty())
  {
    Log::GetLogger()->error("No path was specified!");
    return 1;
  }

  // Load image
  const auto imageName = props.source;
  cv::Mat image = MatIoManager::Instance().GetOrReadMatrixFromImage(imageName);

  // Tests if image was loaded succesfully
  if (image.empty())
  {
    Log::GetLogger()->error("Image at {} not found!", imageName);
    return 2;
  }
  Log::GetLogger()->info("Image at {} loaded!", imageName);

  std::vector<TransformableFace> faces;
  cv::Mat_<cv::Vec3d> faceImage = image;

  if (props.extract)
  {
    faces = FaceExtractionManager::Instance().DetectFace(image);
    if (faces.empty())
    {
      Log::GetLogger()->error("No face was found!");
      return 1;
    }
    faceImage = FaceExtractionManager::Instance().MorphToCanonicalFace(image, faces[0]);
#ifdef NDEBUGDEV
    MatUtil::WriteVectorizedImage("output/WithBeardExtracted.png", faceImage, 3, 95);
#endif
  }

  cv::Mat_<cv::Vec3d> output = Shave(props, faceImage);

  if (props.extractBeard)
  {
    const cv::Mat_<cv::Vec3d> beard = faceImage - output;

    MatUtil::WriteImage("output/beard.png", beard / 2 + 127.5);
    Log::GetLogger()->info("Beard extracted");
  }

  if (props.extract)
  {
#ifdef NDEBUGDEV
    MatUtil::WriteVectorizedImage("output/WithoutBeardExtracted.png", output, 3, 95);
#endif
    output = FaceExtractionManager::Instance().MorphFromCanonicalFace(output, image, faces[0]);
  }

  const auto destination = props.destination;
  MatUtil::WriteImage("output/" + destination, output);
  Log::GetLogger()->info("Image at {} created", destination);

  return 0;
}
