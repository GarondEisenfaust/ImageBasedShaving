#include "ibspch.h"
#include "BeardAdditionManager.h"
#include "ManagerCreatorImpl.h"
#include "BeardAdder.h"
#include "FaceExtractionManager.h"
#include "MatIoManager.h"
#include "ShavingManager.h"
#include "MatrixUtil.h"

REGISTERMANAGERIMPL(BeardAdditionManager, adding);

BeardAdditionManager::BeardAdditionManager() : ManagerBase()
{
}

int BeardAdditionManager::Manage(const Props& props) const
{
  if (props.source.empty())
  {
    Log::GetLogger()->error("No path was specified!");
    return 1;
  }

  // Load image
  const auto imageName = props.source;
  cv::Mat_<cv::Vec3d> image = MatIoManager::Instance().GetOrReadMatrixFromImage(imageName);

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
  }

  cv::Mat_<cv::Vec3d> added;
  if (props.beardedFace.empty())
  {
    const int index = props.index;
    Log::GetLogger()->info("Running beard addition. Adding beard: {}", index);
    added = BeardAdder::Instance().Add(faceImage, index);
  }
  else
  {
    Log::GetLogger()->info("Running beard addition. Getting beard from: {}", props.beardedFace);
    cv::Mat_<cv::Vec3d> beardedFace = MatIoManager::Instance().GetOrReadMatrixFromImage(props.beardedFace);
    cv::Mat_<cv::Vec3d> shaved = ShavingManager().Shave(props, beardedFace);
    cv::Mat_<cv::Vec3d> beard = beardedFace - shaved;
    auto distanceTransformMask = BeardAdder::Instance().DistanceTransform(beard);

    std::vector<cv::Mat_<double>> maskVector;
    maskVector.reserve(3);
    maskVector.push_back(distanceTransformMask);
    maskVector.push_back(distanceTransformMask);
    maskVector.push_back(distanceTransformMask);

    cv::Mat_<cv::Vec3d> distanceTransformMask3Channels;
    cv::merge(maskVector, distanceTransformMask3Channels);

    added = BeardAdder::Instance().Add(faceImage, distanceTransformMask3Channels.mul(beard));
#ifdef NDEBUG
    MatUtil::WriteImage("output/beard.png", beard / 2 + 127.5);
    MatUtil::WriteImage("output/ShavedFace.png", shaved);
#endif
  }

  if (props.extract)
  {
    added = FaceExtractionManager::Instance().MorphFromCanonicalFace(added, image, faces[0]);
  }

  MatUtil::WriteImage("output/" + props.destination, added);
  Log::GetLogger()->info("Image at {} created", props.destination);

  return 0;
}
