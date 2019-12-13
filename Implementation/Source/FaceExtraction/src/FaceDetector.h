#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

typedef std::vector<std::vector<cv::Point2f>> Face;
typedef std::shared_ptr<Face> FacesPointer;
typedef cv::Ptr<cv::face::Facemark> FacemarkPointer;

class FaceDetector
{
public:
  FaceDetector(FaceDetector const&) = delete;
  void operator=(FaceDetector const&) = delete;

private:
  FaceDetector() = default;

public:
  static FaceDetector& Instance();
  FacesPointer DetectFace(FacemarkPointer facemark, cv::Mat& image);
  FacesPointer DetectFace(FacemarkPointer facemark, cv::Mat_<cv::Vec3d>& image);

  void LoadFaceClassifier(cv::String classifier);
  void DrawTrianglesAndPointIndex(cv::Mat& image, FacesPointer landmarks) const;
private:
  cv::CascadeClassifier faceClassifier;
};
