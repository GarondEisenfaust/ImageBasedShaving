#pragma once

class BeardAdder
{
public:
  static BeardAdder& Instance();
  ~BeardAdder() = default;
  BeardAdder(BeardAdder&& other) noexcept = delete;
  BeardAdder(const BeardAdder& other) = delete;
  BeardAdder& operator=(BeardAdder const& other) = delete;
  BeardAdder& operator=(BeardAdder&& other) noexcept = delete;
  cv::Mat_<cv::Vec3d> BeardAdder::Add(const cv::Mat_<cv::Vec3d>& image, int index = 0) const;
  cv::Mat_<double> Add(const cv::Mat_<double>& image, int index = 0) const;
  cv::Mat_<cv::Vec3d> BeardAdder::Add(const cv::Mat_<cv::Vec3d>& image, const cv::Mat_<cv::Vec3d>& beard) const;
  cv::Mat_<double> Add(const cv::Mat_<double>& image, const cv::Mat_<double>& beard) const;
  std::vector<cv::Mat_<double>> ProjectPC(int firstN = 6) const;
  std::vector<cv::Mat_<cv::Vec3d>> GetFirstPcs(int firstN = 6) const;
  cv::Mat_<double> DistanceTransform(const cv::Mat_<cv::Vec3d>& image) const;

private:
  cv::Mat_<double> meanBeard;
  cv::Mat_<double> beards;
  BeardAdder(const std::string& meanBeardMatrix = "Resources/matricies/beardMatSubtractedMeanBeard.yml",
             const std::string& beardMat = "Resources/matricies/meanBeard.yml");
};
