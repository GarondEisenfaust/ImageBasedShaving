#include "ibspch.h"
#include "InputManager.h"

// Use \RebuiltDatabases\StructuredMats\ExactMaskAdaptiveGrabCut\SegNearestNeighbourAdaptive8GrabCut\resultsProcessed\beardMats\results for best results
int main(int argc, const char* argv[])
{
  /*auto u= MatIoManager::Instance().GetOrReadMatrixFromImage("image12060.jpg");
 auto face=  TransformableFace();
 auto z=face.DrawMask();
 cv::Mat_<cv::Vec3d> f;
 u.copyTo(f, z);*/

  /*auto pcs = BeardAdder::Instance().ProjectPC(10);
  std::vector<cv::Mat_<cv::Vec3d>> imagePcs;
  imagePcs.reserve(pcs.size());
  for (auto& pc : pcs)
  {
    imagePcs.push_back(MatIoManager::Instance().DeVectorizeImage<cv::Vec3d, double>(pc, 3, 95));
  }
  auto pcss = BeardAdder::Instance().GetFirstPcs(10);*/

  //cv::Mat_<cv::Vec3d>image = MatIoManager::Instance().GetOrReadMatrixFromYmlFile("124img030.yml");
  //image=image.reshape(3, 95);
  //auto p = BeardExtractor::Instance().GraphCut(image);

  return InputManager::Instance().Manage(argc, argv);
}
