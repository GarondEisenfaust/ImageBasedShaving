#pragma once
#include "ReconstructionAlgorithm.h"

class LayerReconstruction : public ReconstructionAlgorithm
{
public:
  LayerReconstruction(const std::string& faceMatrix = "Resources/matricies/faces.yml",
                             const std::string& beards = "Resources/matricies/beardPcs.yml",
                             const std::string& meanBeardMatrix = "Resources/matricies/meanBeard.yml");
  ~LayerReconstruction() = default;
  LayerReconstruction(LayerReconstruction&& other) noexcept = default;
  LayerReconstruction(const LayerReconstruction& other) = default;
  LayerReconstruction& operator=(LayerReconstruction const& other) = default;
  LayerReconstruction& operator=(LayerReconstruction&& other) noexcept = default;

  cv::Mat_<double> Reconstruct(const cv::Mat_<double>& image) const override;
  virtual cv::Mat_<double> ReconstructionIterate(const cv::Mat_<double>& image) const;

protected:
  const cv::Mat_<double> beards;
  const cv::Mat_<double> meanBeard;
  static void DebugOutput(const cv::Mat_<double>& iteration, const cv::Mat_<double>& face,
                          const cv::Mat_<double>& beard,
                          const cv::Mat_<double>& otherlayer);
};

#ifdef NDEBUGDEV
#define PLOTCOEFFICIENTS(coefficients  ){                                               \
double min;                                                                             \
double max;                                                                             \
int minIndex;                                                                           \
int maxIndex;                                                                           \
cv::minMaxIdx(coefficients, &min, &max, &minIndex, &maxIndex);                          \
const int mostRelevantIndex = abs(maxIndex) > abs(minIndex) ? maxIndex : minIndex;      \
const double mostRelevantValue = abs(max) > abs(min) ? max : min;                       \
auto plot = cv::plot::Plot2d::create(coefficients);                                     \
plot->setPlotBackgroundColor({ 255, 255, 255 });                                        \
plot->setPlotGridColor({ 0, 0, 0 });                                                    \
plot->setPlotAxisColor({ 0, 0, 0 });                                                    \
plot->setPlotTextColor({ 0, 0, 0 });                                                    \
plot->setPlotLineColor({ 0, 0, 255 });                                                  \
plot->setShowText(true);                                                                \
plot->setPointIdxToPrint(mostRelevantIndex);                                            \
plot->setNeedPlotLine(false);                                                           \
plot->setShowGrid(false);                                                               \
plot->setMaxY(mostRelevantValue);                                                       \
plot->setMinY(-mostRelevantValue);                                                      \
cv::Mat plotImage;                                                                      \
plot->render(plotImage);                                                                \
static int i = 0;                                                                       \
MatUtil::WriteImage("output/coefficients"+std::to_string(i)+".png", plotImage);         \
i++;                                                                                    \
}
#else
#define PLOTCOEFFICIENTS(coefficients)
#endif
