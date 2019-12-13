#include "ibspch.h"
#include "LayerReconstruction.h"
#include "MatIoManager.h"
#include "MatrixUtil.h"

LayerReconstruction::
LayerReconstruction(const std::string& faceMatrix, const std::string& beardMatrix,
                           const std::string& meanBeardMatrix) : ReconstructionAlgorithm(faceMatrix), beards(
                                                                   MatIoManager::Instance().GetOrReadMatrixFromYmlFile(
                                                                     beardMatrix)),
                                                                 meanBeard(
                                                                   MatIoManager::Instance().GetOrReadMatrixFromYmlFile(
                                                                     meanBeardMatrix))
{
  //hconcat(beards, meanBeard, beards);
  /*const auto notSplittBeards = MatIoManager::Instance().GetOrReadMatrixFromYmlFile(beardMatrix);
  beards = meanBeard.clone();
  for (int i = 0; i < 3; i++)
  {
    const auto channel = SetAllCannelsToZeroExcept(notSplittBeards, i);
    hconcat(beards, channel, beards);
  }*/
}

cv::Mat_<double> LayerReconstruction::ReconstructionIterate(const cv::Mat_<double>& image) const
{
  cv::Mat_<double> combined;
  hconcat(v, beards, combined);
  static const cv::Mat_<double> regularisation = 0.0 * cv::Mat::eye(combined.cols, combined.cols, CV_64F);
  const cv::Mat_<double> coefficients = (combined.t() * combined + regularisation).inv(cv::DECOMP_CHOLESKY) * (combined
    .t() * (image - meanBeard));

  const cv::Rect faceCoefficientsRect(0, 0, 1, v.cols);
  const cv::Mat_<double> faceVector = v * coefficients(faceCoefficientsRect);

  const cv::Rect beardCoefficientsRect(0, v.cols, 1, beards.cols);
  cv::Mat_<double> beardCoefficients = coefficients(beardCoefficientsRect);
  const cv::Mat_<double> beardVector = beards * beardCoefficients + meanBeard;

  const cv::Mat_<double> otherLayerVector = image - (faceVector + beardVector);

  const cv::Mat_<double> output = faceVector + otherLayerVector;

  DebugOutput(output, faceVector, beardVector, otherLayerVector);
  PLOTCOEFFICIENTS(beardCoefficients);

  return output;
}

cv::Mat_<double> LayerReconstruction::Reconstruct(const cv::Mat_<double>& image) const
{
#ifdef NDEBUGDEV
  MatUtil::WriteVectorizedImage("output/MeanBeard.png", meanBeard / 2 + 127.5);
#endif
  auto resultNew = ReconstructionIterate(image);
  //cv::Mat_<double> resultOld = image.clone();
  //cv::Mat_<double> resultNew;

  //double diff = std::numeric_limits<double>::max();
  /* while (diff > 10)
   {*/
  //resultNew = ReconstructionIterate(resultOld);
  //diff = norm(resultOld - resultNew);
  //Log::GetLogger()->info("Difference : {}", diff);
  //resultOld = resultNew;
  /*}*/
  return resultNew;
}

void LayerReconstruction::DebugOutput(const cv::Mat_<double>& iteration, const cv::Mat_<double>& face,
                                             const cv::Mat_<double>& beard,
                                             const cv::Mat_<double>& otherlayer)
{
#ifdef NDEBUGDEV
  static unsigned int i = 0;
  const cv::Mat_<cv::Vec3d> iterationImage = MatUtil::DeVectorizeImage<cv::Vec3d, double>(iteration, 3, 95);
  const cv::Mat_<cv::Vec3d> faceImage = MatUtil::DeVectorizeImage<cv::Vec3d, double>(face, 3, 95);
  const cv::Mat_<cv::Vec3d> otherLayerImage = MatUtil::DeVectorizeImage<cv::Vec3d, double>(otherlayer, 3, 95);
  const cv::Mat_<cv::Vec3d> beardImage = MatUtil::DeVectorizeImage<cv::Vec3d, double>(beard, 3, 95);

  MatUtil::WriteImage("output/iterationImage_" + std::to_string(i) + ".png", iterationImage);
  MatUtil::WriteImage("output/faceLayer_" + std::to_string(i) + ".png", faceImage);
  MatUtil::WriteImage("output/otherLayer_" + std::to_string(i) + ".png", otherLayerImage / 2 + 127.5);
  MatUtil::WriteImage("output/beardLayer_" + std::to_string(i) + ".png", beardImage / 2 + 127.5);

  i++;
#endif
}
