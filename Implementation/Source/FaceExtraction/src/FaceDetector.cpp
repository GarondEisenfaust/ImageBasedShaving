#include "ibspch.h"
#include "FaceDetector.h"
#include "TransformableFace.h"

FaceDetector& FaceDetector::Instance()
{
  static FaceDetector instance;
  return instance;
}

FacesPointer FaceDetector::DetectFace(FacemarkPointer facemark, cv::Mat& image)
{
  cv::Mat ucharImage;
  image.convertTo(ucharImage, CV_8U);

  cv::Mat gray;
  std::vector<cv::Rect> faces;

  if (ucharImage.channels() == 3)
  {
    cvtColor(ucharImage, gray, cv::COLOR_BGR2GRAY);
    faceClassifier.detectMultiScale(gray, faces);
  }
  else
  {
    faceClassifier.detectMultiScale(ucharImage, faces);
  }

  FacesPointer landmarks = std::make_shared<Face>();

  facemark->fit(ucharImage, faces, *landmarks);

  return landmarks;
}

FacesPointer FaceDetector::DetectFace(FacemarkPointer facemark, cv::Mat_<cv::Vec3d>& image)
{
  cv::Mat ucharImage;
  image.convertTo(ucharImage, CV_8U);
  return DetectFace(facemark, ucharImage);
}

void FaceDetector::LoadFaceClassifier(cv::String classifier)
{
  this->faceClassifier = cv::CascadeClassifier(classifier);
}

void FaceDetector::DrawTrianglesAndPointIndex(cv::Mat& image, FacesPointer landmarks) const
{
  for (auto& points : *landmarks)
  {
    TransformableFace face(points);
    face.DrawAllTriangles(image);
    face.DrawAllPoints(image);
  }
}
