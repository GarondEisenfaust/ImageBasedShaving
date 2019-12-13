#pragma once
#include "TransformableFace.h"

class FaceMorpher
{
public:
  FaceMorpher(FaceMorpher const&) = delete;
  void operator=(FaceMorpher const&) = delete;
  static FaceMorpher& getInstance();
  void SetCanonicalFace(TransformableFace canonicalFace);
  void MorphToCanonicalFace(cv::Mat& sourceImage, cv::Mat& targetImage, TransformableFace& sourceFace) const;
  void MorphFromCanonicalFace(cv::Mat& sourceImage, cv::Mat& targetImage, TransformableFace& formerFace) const;

private:
  static void MorphFace(cv::Mat& sourceImage, cv::Mat& targetImage, TransformableFace& sourceFace,
                        TransformableFace& targetFace);
  static void MorphTriangle(cv::Mat& sourceImage, cv::Mat& targetImage, Triangle& sourceTriangle,
                            Triangle& targetTriangle);
  std::unique_ptr<TransformableFace> canonicalFace;
  FaceMorpher() = default;
};
