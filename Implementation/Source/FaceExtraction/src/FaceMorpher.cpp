#include "ibspch.h"
#include "FaceMorpher.h"

FaceMorpher& FaceMorpher::getInstance()
{
  static FaceMorpher instance;
  return instance;
}

void FaceMorpher::SetCanonicalFace(TransformableFace canonicalFace)
{
  this->canonicalFace.reset();
  this->canonicalFace = std::make_unique<TransformableFace>(canonicalFace);
}

void FaceMorpher::MorphToCanonicalFace(cv::Mat& sourceImage, cv::Mat& targetImage, TransformableFace& sourceFace) const
{
  MorphFace(sourceImage, targetImage, sourceFace, *canonicalFace);
}

void FaceMorpher::MorphFromCanonicalFace(cv::Mat& sourceImage, cv::Mat& targetImage,
                                         TransformableFace& formerFace) const
{
  MorphFace(sourceImage, targetImage, *canonicalFace, formerFace);
}

void FaceMorpher::MorphTriangle(cv::Mat& sourceImage, cv::Mat& targetImage, Triangle& sourceTriangle,
                                Triangle& targetTriangle)
{
  std::array<cv::Point2f, 3> sourceTrianglePoints = sourceTriangle.ToArray();
  std::array<cv::Point2f, 3> targetTrianglePoints = targetTriangle.ToArray();

  // Find bounding rectangle for each triangle
  const cv::Rect sourceRect = boundingRect(sourceTrianglePoints);
  const cv::Rect targetRect = boundingRect(targetTrianglePoints);

  // Offset points by left top corner of the respective rectangles
  std::array<cv::Point2f, 3> triSourceCropped, triTargetCropped;
  std::array<cv::Point, 3> triTargetCroppedInt;
  for (int i = 0; i < 3; i++)
  {
    triSourceCropped[i] = (cv::Point2f(sourceTrianglePoints[i].x - sourceRect.x,
                                       sourceTrianglePoints[i].y - sourceRect.y));
    triTargetCropped[i] = (cv::Point2f(targetTrianglePoints[i].x - targetRect.x,
                                       targetTrianglePoints[i].y - targetRect.y));

    // fillConvexPoly needs a vector of Point and not Point2f
    triTargetCroppedInt[i] = (cv::Point(static_cast<int>(triTargetCropped[i].x),
                                        static_cast<int>(triTargetCropped[i].y)));
  }

  // Apply warpImage to small rectangular patches
  cv::Mat imgSourceCropped;
  sourceImage(sourceRect).copyTo(imgSourceCropped);

  // Given a pair of triangles, find the affine transform.
  const cv::Mat warpMat = getAffineTransform(triSourceCropped, triTargetCropped);

  // Apply the Affine Transform just found to the src image
  cv::Mat imgTargetCropped = cv::Mat::zeros(targetRect.height, targetRect.width, imgSourceCropped.type());
  warpAffine(imgSourceCropped, imgTargetCropped, warpMat, imgTargetCropped.size(), cv::INTER_LINEAR,
             cv::BORDER_REFLECT_101);

  // Get mask by filling triangle
  cv::Mat mask = cv::Mat::zeros(targetRect.height, targetRect.width, CV_64FC3);
  fillConvexPoly(mask, triTargetCroppedInt, cv::Scalar(1.0, 1.0, 1.0), 16, 0);

  // Copy triangular region of the rectangular patch to the output image
  multiply(imgTargetCropped, mask, imgTargetCropped);
  cv::Mat temp = cv::Scalar(1.0, 1.0, 1.0) - mask;
  multiply(targetImage(targetRect), temp, targetImage(targetRect));
  targetImage(targetRect) = targetImage(targetRect) + imgTargetCropped;
}

void FaceMorpher::MorphFace(cv::Mat& sourceImage, cv::Mat& targetImage, TransformableFace& sourceFace,
                            TransformableFace& targetFace)
{
  std::array<Triangle, 107>& sourceTriangles = sourceFace.GetTriangles();
  std::array<Triangle, 107>& targetTriangles = targetFace.GetTriangles();

  for (unsigned int i = 0; i < sourceTriangles.size(); i++)
  {
    MorphTriangle(sourceImage, targetImage, sourceTriangles[i], targetTriangles[i]);
  }
}
