#pragma once
#include "ibspch.h"


struct Triangle
{
  cv::Point2f p_1, p_2, p_3;

  std::array<cv::Point2f, 3> ToArray() const
  {
    std::array<cv::Point2f, 3> points = {p_1, p_2, p_3};
    return points;
  }

  ~Triangle() = default;
  Triangle& operator=(const Triangle& other) = default;
};

class TransformableFace
{
public:
  TransformableFace();
  TransformableFace(const TransformableFace& old_face);
  TransformableFace(std::vector<cv::Point2f>& face);
  ~TransformableFace() = default;

  void DrawAllTriangles(cv::Mat& image, const cv::Scalar& color = {0, 255, 0}) const;
  void DrawAllPoints(cv::Mat& image, const cv::Scalar& color = {0, 255, 0}) const;
  void FillAllTriangles(cv::Mat& image, const cv::Scalar& color = {0, 255, 0}) const;
  cv::Mat_<uchar> DrawMask(const cv::Scalar& color = 1) const;
  cv::Mat_<uchar> DrawMaskWithoutMouth(const cv::Scalar& color = 1) const;
  cv::Mat_<uchar> DrawWholeFaceMask(const cv::Scalar& color = 1) const;

  static std::vector<cv::Point2f> CreateDefaultFace();
  std::array<Triangle, 107>& GetTriangles();

  TransformableFace& operator=(TransformableFace const& rhs)
  {
    if (this != &rhs)
    {
      TransformableFace tmp(rhs);
      std::swap(*this, tmp);
    }
    return *this;
  }

private:
  std::unique_ptr<std::array<Triangle, 107>> triangles;
  std::unique_ptr<std::vector<cv::Point2f>> points;
  static void DrawTriangle(cv::Mat& image, const Triangle& triangle, const cv::Scalar& color = {0, 255, 0});
  static void FillTriangle(cv::Mat& image, const Triangle& triangle, const cv::Scalar& color = {0, 255, 0});
  static void SetManualPixels(cv::Mat_<uchar>& mask);
  static std::unique_ptr<std::array<Triangle, 107>> CreateTriangles(std::vector<cv::Point2f>& face);
};
