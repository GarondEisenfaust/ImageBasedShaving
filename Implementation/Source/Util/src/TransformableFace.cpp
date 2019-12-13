#include "ibspch.h"
#include "Exports/TransformableFace.h"

TransformableFace::TransformableFace(std::vector<cv::Point2f>& face)
{
  points = std::make_unique<std::vector<cv::Point2f>>(face);
  triangles = CreateTriangles(face);
}

TransformableFace::TransformableFace()
{
  points = std::make_unique<std::vector<cv::Point2f>>(CreateDefaultFace());
  triangles = CreateTriangles(*points);
}

TransformableFace::TransformableFace(const TransformableFace& old_face)
{
  triangles.reset();
  triangles = std::make_unique<std::array<Triangle, 107>>(*(old_face.triangles));
}

void TransformableFace::DrawTriangle(cv::Mat& image, const Triangle& triangle, const cv::Scalar& color)
{
  line(image, triangle.p_1, triangle.p_2, color, 1);
  line(image, triangle.p_2, triangle.p_3, color, 1);
  line(image, triangle.p_3, triangle.p_1, color, 1);
}

void TransformableFace::FillTriangle(cv::Mat& image, const Triangle& triangle, const cv::Scalar& color)
{
  cv::Point2i points[3] = {triangle.p_2, triangle.p_1, triangle.p_3};
  fillConvexPoly(image, points, 3, color);
}

void TransformableFace::SetManualPixels(cv::Mat_<uchar>& mask)
{
  // manual setting of pixels that are for some reason not 0
  mask(5, 25) = 0;
  mask(5, 26) = 0;
  mask(5, 27) = 0;
  mask(5, 28) = 0;
  mask(5, 29) = 0;
  mask(5, 30) = 0;
  mask(5, 31) = 0;
  mask(5, 32) = 0;
  mask(5, 33) = 0;
  mask(5, 34) = 0;
}

void TransformableFace::DrawAllTriangles(cv::Mat& image, const cv::Scalar& color) const
{
  for (auto& triangle : *triangles)
  {
    DrawTriangle(image, triangle, color);
  }
}

void TransformableFace::DrawAllPoints(cv::Mat& image, const cv::Scalar& color) const
{
  for (unsigned int i = 0; i < points->size(); i++)
  {
    static cv::Scalar black(0, 0, 0);
    const cv::Point2f point_num_position(points->at(i).x + 3, points->at(i).y);
    circle(image, points->at(i), 1, color, cv::FILLED);
    putText(image, std::to_string(i), point_num_position, cv::FONT_HERSHEY_PLAIN, 0.5, black);
  }
}

void TransformableFace::FillAllTriangles(cv::Mat& image, const cv::Scalar& color) const
{
  for (auto& triangle : *triangles)
  {
    FillTriangle(image, triangle, color);
  }
}

cv::Mat_<uchar> TransformableFace::DrawMask(const cv::Scalar& color) const
{
  cv::Mat_<uchar> mask = cv::Mat::zeros(95, 95, CV_8U);
  FillAllTriangles(mask, 1);
  std::vector<cv::Point2i> mouth;
  mouth.reserve(12);

  mouth.push_back(points->at(48));
  mouth.push_back(points->at(49));
  mouth.push_back(points->at(50));
  mouth.push_back(points->at(51));
  mouth.push_back(points->at(52));
  mouth.push_back(points->at(53));
  mouth.push_back(points->at(54));
  mouth.push_back(points->at(55));
  mouth.push_back(points->at(56));
  mouth.push_back(points->at(57));
  mouth.push_back(points->at(58));
  mouth.push_back(points->at(59));
  cv::fillConvexPoly(mask, mouth, 0);

  std::vector<cv::Point2i> upperFace;
  upperFace.reserve(17);

  upperFace.push_back(points->at(17));
  upperFace.push_back(points->at(18));
  upperFace.push_back(points->at(19));
  upperFace.push_back(points->at(20));
  upperFace.push_back(points->at(21));
  upperFace.push_back(points->at(22));
  upperFace.push_back(points->at(23));
  upperFace.push_back(points->at(24));
  upperFace.push_back(points->at(25));
  upperFace.push_back(points->at(26));
  upperFace.push_back(points->at(16));
  upperFace.push_back(points->at(15));
  upperFace.push_back(points->at(35));
  upperFace.push_back(points->at(34));
  upperFace.push_back(points->at(33));
  upperFace.push_back(points->at(32));
  upperFace.push_back(points->at(31));
  upperFace.push_back(points->at(1));
  upperFace.push_back(points->at(0));
  cv::fillConvexPoly(mask, upperFace, 0);
  SetManualPixels(mask);

  return mask;
}

cv::Mat_<uchar> TransformableFace::DrawMaskWithoutMouth(const cv::Scalar& color) const
{
  cv::Mat_<uchar> mask = cv::Mat::zeros(95, 95, CV_8U);
  FillAllTriangles(mask, 1);

  std::vector<cv::Point2i> upperFace;
  upperFace.reserve(17);

  upperFace.push_back(points->at(17));
  upperFace.push_back(points->at(18));
  upperFace.push_back(points->at(19));
  upperFace.push_back(points->at(20));
  upperFace.push_back(points->at(21));
  upperFace.push_back(points->at(22));
  upperFace.push_back(points->at(23));
  upperFace.push_back(points->at(24));
  upperFace.push_back(points->at(25));
  upperFace.push_back(points->at(26));
  upperFace.push_back(points->at(16));
  upperFace.push_back(points->at(15));
  upperFace.push_back(points->at(35));
  upperFace.push_back(points->at(34));
  upperFace.push_back(points->at(33));
  upperFace.push_back(points->at(32));
  upperFace.push_back(points->at(31));
  upperFace.push_back(points->at(1));
  upperFace.push_back(points->at(0));
  cv::fillConvexPoly(mask, upperFace, 0);
  SetManualPixels(mask);

  return mask;
}

cv::Mat_<uchar> TransformableFace::DrawWholeFaceMask(const cv::Scalar& color) const
{
  cv::Mat_<uchar> mask = cv::Mat::zeros(95, 95, CV_8U);
  FillAllTriangles(mask, color);
  return mask;
}

std::array<Triangle, 107>& TransformableFace::GetTriangles()
{
  return *triangles;
}

std::unique_ptr<std::array<Triangle, 107>> TransformableFace::CreateTriangles(std::vector<cv::Point2f>& face)
{
  std::unique_ptr<std::array<Triangle, 107>> triangles = std::make_unique<std::array<Triangle, 107>>();
  triangles->at(0) = {face[0], face[1], face[36]};
  triangles->at(1) = {face[1], face[2], face[36]};
  triangles->at(2) = {face[2], face[3], face[36]};
  triangles->at(3) = {face[3], face[31], face[36]};
  triangles->at(4) = {face[3], face[49], face[31]};
  triangles->at(5) = {face[3], face[48], face[49]};
  triangles->at(6) = {face[3], face[4], face[48]};
  triangles->at(7) = {face[4], face[5], face[48]};
  triangles->at(8) = {face[5], face[59], face[48]};
  triangles->at(9) = {face[5], face[6], face[59]};
  triangles->at(10) = {face[6], face[58], face[59]};
  triangles->at(11) = {face[6], face[7], face[58]};
  triangles->at(12) = {face[7], face[57], face[58]};
  triangles->at(13) = {face[7], face[8], face[57]};
  triangles->at(14) = {face[8], face[9], face[57]};
  triangles->at(15) = {face[9], face[56], face[57]};
  triangles->at(16) = {face[9], face[10], face[56]};
  triangles->at(17) = {face[10], face[55], face[56]};
  triangles->at(18) = {face[10], face[11], face[55]};
  triangles->at(19) = {face[11], face[54], face[55]};
  triangles->at(20) = {face[11], face[12], face[54]};
  triangles->at(21) = {face[12], face[13], face[54]};
  triangles->at(22) = {face[13], face[53], face[54]};
  triangles->at(23) = {face[13], face[35], face[53]};
  triangles->at(24) = {face[13], face[45], face[35]};
  triangles->at(25) = {face[13], face[14], face[45]};
  triangles->at(26) = {face[14], face[15], face[45]};
  triangles->at(27) = {face[15], face[16], face[45]};
  triangles->at(28) = {face[16], face[26], face[45]};
  triangles->at(29) = {face[26], face[25], face[45]};
  triangles->at(30) = {face[25], face[44], face[45]};
  triangles->at(31) = {face[25], face[24], face[44]};
  triangles->at(32) = {face[24], face[43], face[44]};
  triangles->at(33) = {face[23], face[43], face[24]};
  triangles->at(34) = {face[23], face[42], face[43]};
  triangles->at(35) = {face[22], face[42], face[23]};
  triangles->at(36) = {face[22], face[27], face[42]};
  triangles->at(37) = {face[21], face[27], face[22]};
  triangles->at(38) = {face[21], face[39], face[27]};
  triangles->at(39) = {face[20], face[21], face[39]};
  triangles->at(40) = {face[20], face[38], face[39]};
  triangles->at(41) = {face[19], face[38], face[20]};
  triangles->at(42) = {face[19], face[37], face[38]};
  triangles->at(43) = {face[18], face[37], face[19]};
  triangles->at(44) = {face[18], face[36], face[37]};
  triangles->at(45) = {face[17], face[36], face[18]};
  triangles->at(46) = {face[0], face[36], face[17]};
  triangles->at(47) = {face[36], face[41], face[37]};
  triangles->at(48) = {face[37], face[41], face[40]};
  triangles->at(49) = {face[37], face[40], face[38]};
  triangles->at(50) = {face[38], face[40], face[39]};
  triangles->at(51) = {face[39], face[28], face[27]};
  triangles->at(52) = {face[27], face[28], face[42]};
  triangles->at(53) = {face[42], face[47], face[43]};
  triangles->at(54) = {face[43], face[47], face[44]};
  triangles->at(55) = {face[44], face[47], face[46]};
  triangles->at(56) = {face[44], face[46], face[45]};
  triangles->at(57) = {face[36], face[31], face[41]};
  triangles->at(58) = {face[41], face[31], face[30]};
  triangles->at(59) = {face[41], face[30], face[40]};
  triangles->at(60) = {face[40], face[30], face[29]};
  triangles->at(61) = {face[40], face[29], face[39]};
  triangles->at(62) = {face[39], face[29], face[28]};
  triangles->at(63) = {face[28], face[29], face[42]};
  triangles->at(64) = {face[42], face[29], face[47]};
  triangles->at(65) = {face[47], face[29], face[30]};
  triangles->at(66) = {face[47], face[30], face[46]};
  triangles->at(67) = {face[46], face[30], face[35]};
  triangles->at(68) = {face[46], face[35], face[45]};
  triangles->at(69) = {face[30], face[31], face[32]};
  triangles->at(70) = {face[30], face[32], face[33]};
  triangles->at(71) = {face[30], face[33], face[34]};
  triangles->at(72) = {face[30], face[34], face[35]};
  triangles->at(73) = {face[31], face[49], face[50]};
  triangles->at(74) = {face[31], face[50], face[32]};
  triangles->at(75) = {face[32], face[50], face[51]};
  triangles->at(76) = {face[32], face[51], face[33]};
  triangles->at(77) = {face[33], face[51], face[34]};
  triangles->at(78) = {face[34], face[51], face[52]};
  triangles->at(79) = {face[34], face[52], face[35]};
  triangles->at(80) = {face[35], face[52], face[53]};
  triangles->at(81) = {face[50], face[49], face[61]};
  triangles->at(82) = {face[50], face[61], face[62]};
  triangles->at(83) = {face[50], face[62], face[51]};
  triangles->at(84) = {face[51], face[62], face[52]};
  triangles->at(85) = {face[52], face[62], face[63]};
  triangles->at(86) = {face[52], face[63], face[53]};
  triangles->at(87) = {face[49], face[48], face[60]};
  triangles->at(88) = {face[49], face[60], face[61]};
  triangles->at(89) = {face[61], face[60], face[67]};
  triangles->at(90) = {face[61], face[67], face[66]};
  triangles->at(91) = {face[61], face[66], face[62]};
  triangles->at(92) = {face[62], face[66], face[63]};
  triangles->at(93) = {face[63], face[66], face[65]};
  triangles->at(94) = {face[63], face[65], face[64]};
  triangles->at(95) = {face[53], face[63], face[64]};
  triangles->at(96) = {face[53], face[64], face[54]};
  triangles->at(97) = {face[48], face[59], face[60]};
  triangles->at(98) = {face[60], face[59], face[67]};
  triangles->at(99) = {face[67], face[59], face[58]};
  triangles->at(100) = {face[67], face[58], face[66]};
  triangles->at(101) = {face[66], face[58], face[57]};
  triangles->at(102) = {face[66], face[57], face[56]};
  triangles->at(103) = {face[65], face[66], face[56]};
  triangles->at(104) = {face[65], face[56], face[55]};
  triangles->at(105) = {face[65], face[55], face[64]};
  triangles->at(106) = {face[64], face[55], face[54]};
  return triangles;
}

std::vector<cv::Point2f> TransformableFace::CreateDefaultFace()
{
  std::vector<cv::Point2f> points;
  points.emplace_back<cv::Point2f>({3, 25});
  points.emplace_back<cv::Point2f>({4, 36});
  points.emplace_back<cv::Point2f>({5, 48});
  points.emplace_back<cv::Point2f>({8, 59});
  points.emplace_back<cv::Point2f>({13, 69});
  points.emplace_back<cv::Point2f>({21, 78});
  points.emplace_back<cv::Point2f>({29, 85});
  points.emplace_back<cv::Point2f>({39, 91});
  points.emplace_back<cv::Point2f>({50, 93});
  points.emplace_back<cv::Point2f>({61, 90});
  points.emplace_back<cv::Point2f>({70, 84});
  points.emplace_back<cv::Point2f>({78, 76});
  points.emplace_back<cv::Point2f>({84, 66});
  points.emplace_back<cv::Point2f>({88, 56});
  points.emplace_back<cv::Point2f>({90, 45});
  points.emplace_back<cv::Point2f>({91, 33});
  points.emplace_back<cv::Point2f>({91, 22});
  points.emplace_back<cv::Point2f>({16, 9});
  points.emplace_back<cv::Point2f>({21, 5});
  points.emplace_back<cv::Point2f>({27, 4});
  points.emplace_back<cv::Point2f>({33, 4});
  points.emplace_back<cv::Point2f>({40, 6});
  points.emplace_back<cv::Point2f>({57, 5});
  points.emplace_back<cv::Point2f>({63, 4});
  points.emplace_back<cv::Point2f>({69, 3});
  points.emplace_back<cv::Point2f>({75, 4});
  points.emplace_back<cv::Point2f>({80, 7});
  points.emplace_back<cv::Point2f>({49, 17});
  points.emplace_back<cv::Point2f>({49, 24});
  points.emplace_back<cv::Point2f>({50, 30});
  points.emplace_back<cv::Point2f>({50, 37});
  points.emplace_back<cv::Point2f>({43, 43});
  points.emplace_back<cv::Point2f>({46, 44});
  points.emplace_back<cv::Point2f>({50, 45});
  points.emplace_back<cv::Point2f>({54, 44});
  points.emplace_back<cv::Point2f>({57, 43});
  points.emplace_back<cv::Point2f>({24, 19});
  points.emplace_back<cv::Point2f>({28, 17});
  points.emplace_back<cv::Point2f>({33, 17});
  points.emplace_back<cv::Point2f>({37, 19});
  points.emplace_back<cv::Point2f>({33, 20});
  points.emplace_back<cv::Point2f>({28, 20});
  points.emplace_back<cv::Point2f>({60, 18});
  points.emplace_back<cv::Point2f>({64, 16});
  points.emplace_back<cv::Point2f>({69, 16});
  points.emplace_back<cv::Point2f>({72, 18});
  points.emplace_back<cv::Point2f>({69, 20});
  points.emplace_back<cv::Point2f>({64, 19});
  points.emplace_back<cv::Point2f>({35, 60});
  points.emplace_back<cv::Point2f>({39, 56});
  points.emplace_back<cv::Point2f>({44, 53});
  points.emplace_back<cv::Point2f>({50, 53});
  points.emplace_back<cv::Point2f>({56, 53});
  points.emplace_back<cv::Point2f>({61, 56});
  points.emplace_back<cv::Point2f>({65, 60});
  points.emplace_back<cv::Point2f>({61, 65});
  points.emplace_back<cv::Point2f>({56, 69});
  points.emplace_back<cv::Point2f>({50, 70});
  points.emplace_back<cv::Point2f>({44, 69});
  points.emplace_back<cv::Point2f>({39, 65});
  points.emplace_back<cv::Point2f>({39, 60});
  points.emplace_back<cv::Point2f>({44, 58});
  points.emplace_back<cv::Point2f>({50, 57});
  points.emplace_back<cv::Point2f>({56, 58});
  points.emplace_back<cv::Point2f>({61, 60});
  points.emplace_back<cv::Point2f>({56, 62});
  points.emplace_back<cv::Point2f>({50, 64});
  points.emplace_back<cv::Point2f>({44, 63});
  return points;
}
