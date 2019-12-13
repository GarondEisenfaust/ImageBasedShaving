#include "ibspch.h"
#include "Exports/FaceExtractionManager.h"
#include "FaceExtraction.h"

FaceExtractionManager& FaceExtractionManager::Instance()
{
  static FaceExtractionManager instance;
  return instance;
}

std::vector<TransformableFace> FaceExtractionManager::DetectFace(cv::Mat& image) const
{
  std::vector<TransformableFace> faces;
  try
  {
    FacesPointer landmarks = FaceDetector::Instance().DetectFace(facemark, image);

    for (auto& face : *landmarks)
    {
      faces.emplace_back(TransformableFace(face));
    }
  }
  catch (cv::Exception e)
  {
    std::cout << e.msg << "\n";
  }
  return faces;
}

cv::Mat FaceExtractionManager::MorphToCanonicalFace(cv::Mat& sourceImage,
                                                    TransformableFace& sourceFace) const
{
  cv::Mat output = cv::Mat::zeros(95, 95, sourceImage.type());
  FaceMorpher::getInstance().MorphToCanonicalFace(sourceImage, output, sourceFace);
  return output;
}

cv::Mat FaceExtractionManager::MorphFromCanonicalFace(cv::Mat& canonicalFaceImage,
                                                      cv::Mat& sourceImage,
                                                      TransformableFace& formerFace) const
{
  canonicalFaceImage = PostProcess(canonicalFaceImage);
  cv::Mat output = sourceImage.clone();
  FaceMorpher::getInstance().MorphFromCanonicalFace(canonicalFaceImage, output, formerFace);
  return output;
}

FaceExtractionManager::FaceExtractionManager()
{
  // Creates the canonical face and sets it as canonicalFace of the FaceMorpher
  std::vector<cv::Point2f> canonicalFacePoints = TransformableFace::CreateDefaultFace();
  TransformableFace canonicalFace(canonicalFacePoints);
  FaceMorpher::getInstance().SetCanonicalFace(canonicalFace);
  FaceDetector::Instance().LoadFaceClassifier("./Resources/detection/haarcascade_frontalface_alt2.xml");
  facemark = cv::face::createFacemarkLBF();
  facemark->loadModel("./Resources/detection/lbfmodel.yaml");
}

const cv::Mat& FaceExtractionManager::PostProcess(cv::Mat& image)
{
  cv::Mat mask = TransformableFace().DrawWholeFaceMask();
  cv::Scalar dominantColor = CalculateDominantColor(image);
  cv::Mat background(95, 95, CV_64FC3, dominantColor);
  background.copyTo(image, 1 - mask);
  return image;
}

cv::Scalar FaceExtractionManager::CalculateDominantColor(cv::Mat& image, unsigned int numSeeds, float threshold)
{
  cv::Mat labels;
  cv::Mat centers;
  cv::Mat imageFloat;
  image.convertTo(imageFloat, CV_32F);
  imageFloat = imageFloat.reshape(3, imageFloat.cols * imageFloat.rows);

  kmeans(imageFloat, numSeeds, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0), 3,
         cv::KMEANS_PP_CENTERS, centers);
  for (unsigned int i = 0; i < numSeeds; i++)
  {
    cv::Mat dominantColorRow = centers.row(i);
    cv::Scalar dominantColor(dominantColorRow.at<float>(0), dominantColorRow.at<float>(1),
                             dominantColorRow.at<float>(2));
    if (dominantColor[0] > threshold && dominantColor[1] > threshold && dominantColor[2] > threshold)
    {
      return dominantColor;
    }
  }
  return {128, 155, 219};
}
