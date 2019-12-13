#pragma once

#define REGISTERIMPL(classname,name) const ReconstructionCreatorImpl<classname> name(#name);

class ReconstructionAlgorithm
{
public:
  ReconstructionAlgorithm(const std::string& path = "Resources/matricies/faces.yml");
  virtual ~ReconstructionAlgorithm() = default;
  ReconstructionAlgorithm(ReconstructionAlgorithm&& other) noexcept = default;
  ReconstructionAlgorithm(const ReconstructionAlgorithm& other) = default;
  ReconstructionAlgorithm& operator=(ReconstructionAlgorithm const& other) = default;
  ReconstructionAlgorithm& operator=(ReconstructionAlgorithm&& other) = default;

  virtual cv::Mat_<double> Reconstruct(const cv::Mat_<double>& image) const = 0;
protected:
  const cv::Mat_<double> v;
};
