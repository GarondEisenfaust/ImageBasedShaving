#pragma once

namespace MatUtil
{
  cv::Mat_<cv::Vec3d> ReadImageAndConvertTodouble(const std::filesystem::path& imageName);
  cv::Mat_<double> ReadMatrixFromFile(const std::filesystem::path& path);
  void WriteVectorizedImage(const std::string& name, const cv::Mat_<double>& vectorizedImage, int channels = 3,
                            int rows = 95);
  void WriteImage(const std::string& name, const cv::Mat_<cv::Vec3d>& image);

  template <class T, class ChannelT>
  cv::Mat_<T> VectorizeImage(const cv::Mat_<ChannelT>& image)
  {
    return image.reshape(1, image.rows * image.cols * image.channels());
  }

  template <class ChannelT, class T>
  cv::Mat_<ChannelT> DeVectorizeImage(const cv::Mat_<T>& image, int channels, int rows)
  {
    return image.reshape(channels, rows);
  }
}
