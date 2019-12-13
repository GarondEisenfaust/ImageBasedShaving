#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "TransformableFace.h"

typedef std::vector<std::vector<cv::Point2f>> Face;
typedef std::shared_ptr<Face> FacesPointer;
typedef cv::Ptr<cv::face::Facemark> FacemarkPointer;

class FaceExtractionManager
{
public:
  FaceExtractionManager(FaceExtractionManager const&) = delete;
  void operator=(FaceExtractionManager const&) = delete;
  FaceExtractionManager(FaceExtractionManager&&) = delete;
  FaceExtractionManager& operator =(FaceExtractionManager&&) = delete;
  ~FaceExtractionManager() = default;
  static FaceExtractionManager& Instance();
  std::vector<TransformableFace> DetectFace(cv::Mat& image) const;

  cv::Mat MorphToCanonicalFace(cv::Mat& sourceImage, TransformableFace& sourceFace) const;
  cv::Mat MorphFromCanonicalFace(cv::Mat& canonicalFaceImage, cv::Mat& sourceImage,
                                 TransformableFace& formerFace) const;
private:
  FaceExtractionManager();
  static const cv::Mat& PostProcess(cv::Mat& image);
  static cv::Scalar CalculateDominantColor(cv::Mat& image, unsigned int numSeeds = 4, float threshold = 10);
  FacemarkPointer facemark;
};
