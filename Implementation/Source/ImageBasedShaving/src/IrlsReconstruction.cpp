#include "ibspch.h"
#include "FaceExtractionManager.h"
#include "IrlsReconstruction.h"
#include "MatrixUtil.h"

const double lambdaSquared = 0.001;

IrlsReconstruction::IrlsReconstruction(const std::string& path) : ReconstructionAlgorithm(path)
{
}

cv::Mat_<double> IrlsReconstruction::Reconstruct(const cv::Mat_<double>& image) const
{
  return Reconstruct(image, CalculateReconstructionCoefficients);
}

double calcEnergy(const cv::Mat_<double>& image, const cv::Mat_<double>& reconstructed, const double sigmaSquared)
{
  cv::Mat_<double> p(image.rows, 1);
  for (int i = 0; i < image.rows; i++)
  {
    const double xSquared = (image.at<double>(i) - reconstructed.at<double>(i)) * (image.at<double>(i) - reconstructed.
      at<double>(i));
    p.at<double>(i) = xSquared / (xSquared + sigmaSquared);
    //p.at<double>(i) = sqrtf(xSquared + static_cast<double>(0.1));
    //p.at<double>(i) = 2 * lambdaSquared * sqrt(1 + xSquared / lambdaSquared);
  }
  return norm(p, cv::NORM_L1);
}

cv::Mat_<double> IrlsReconstruction::Reconstruct(const cv::Mat_<double>& image,
                                                 const std::function<cv::Mat_<double>(
                                                   const cv::Mat_<double>&, const cv::Mat_<double>&,
                                                   const cv::Mat_<double>&)>& calculateReconstructionCoefficients) const
{
  auto mask = CreateVectorMask();

  cv::Mat_<double> w;
  //cv::Mat(cv::Mat::ones(v.rows, 1, CV_64F)).copyTo(w,mask);
  //CalculateInitialWNearestNeighbour(image).copyTo(w, mask);
  CalculateInitialWMeanFace(image).copyTo(w, mask);

  cv::Mat_<double> cOld = cv::Mat::ones(v.cols, 1, CV_64F);
  cv::Mat_<double> reconstructed;
  cv::Mat_<double> residual;
  double diff = std::numeric_limits<double>::max();
  double sigmaSquared = 0.0;
  unsigned int i = 0;

  while (diff > 0.005)
  {
    cv::Mat_<double> cNew = calculateReconstructionCoefficients(v, w, image);
    reconstructed = v * cNew;

    residual = abs(image - reconstructed);

    Log::GetLogger()->info("Iteration: {}  difference: {}  sigmasquared: {}  weighted: {}  energy: {}", i, diff,
                           sigmaSquared, norm(WeightMatrix(w, residual), cv::NORM_L1),
                           calcEnergy(image, reconstructed, sigmaSquared));

    DebugOutput(reconstructed, cNew, w, i);

    sigmaSquared = CalculateSigmaSquared(residual);
    CalculateW(residual, sigmaSquared).copyTo(w, mask);
    diff = norm(cOld - cNew);

    cOld = cNew;
    i++;
  }
  cv::Mat_<double> reconstructedWithMask;
  reconstructed.copyTo(reconstructedWithMask, mask);
  return reconstructedWithMask;
}

cv::Mat_<uchar> IrlsReconstruction::CreateVectorMask()
{
  const auto oneChannelMask = TransformableFace().DrawWholeFaceMask();
  std::vector<cv::Mat_<uchar>> channelArray;
  channelArray.reserve(3);
  channelArray.push_back(oneChannelMask);
  channelArray.push_back(oneChannelMask);
  channelArray.push_back(oneChannelMask);

  cv::Mat_<cv::Vec3b> threeChannelMask;
  cv::merge(channelArray, threeChannelMask);
  return MatUtil::VectorizeImage<uchar, cv::Vec3b>(threeChannelMask);
}

cv::Mat_<double> IrlsReconstruction::CalculateReconstructionCoefficients(const cv::Mat_<double>& v,
                                                                         const cv::Mat_<double>& w,
                                                                         const cv::Mat_<double>& image)
{
  const cv::Mat_<double> wSqared = w.mul(w);
  static const cv::Mat_<double> regularisation = 0.0 * cv::Mat::eye(v.cols, v.cols, CV_64F);
  return ((v.t() * WeightMatrix(wSqared, v)) + regularisation).inv(cv::DECOMP_CHOLESKY) * (v.t() *
    WeightMatrix(wSqared, image));
}

double IrlsReconstruction::CalculateSigmaSquared(const cv::Mat_<double>& residual)
{
  static double magicNumber = 1.4826;
  const double median = CalculateQuantile(residual);
  const double sigma = magicNumber * median;
  return sigma * sigma;
}

double IrlsReconstruction::CalculateQuantile(const cv::Mat_<double>& image, double part)
{
  const auto offset = std::lround(image.total() * part);
  std::vector<double> vectorFromMat;
  image.copyTo(vectorFromMat);
  std::nth_element(vectorFromMat.begin(), vectorFromMat.begin() + offset, vectorFromMat.end());
  return vectorFromMat[offset];
}

double IrlsReconstruction::CalculateMedianHist(const cv::Mat_<double>& image)
{
  double m = (image.rows * image.cols) / 2;
  int bin = 0;
  double med = -1.0;

  const int histSize = 256;
  const float range[] = {0, 256};
  const float* histRange = {range};
  const bool uniform = true;
  const bool accumulate = false;
  cv::Mat hist;

  calcHist(&image, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

  for (int i = 0; i < histSize && med < 0.0; ++i)
  {
    bin += cvRound(hist.at<double>(i));
    if (bin > m && med < 0.0)
    {
      med = i;
    }
  }
  return med;
}

template <class T>
struct WCalculatorOperator
{
  cv::Mat_<T>& residual;
  double sigmaSquared;

  WCalculatorOperator(cv::Mat_<T>& residual, double sigmaSquared) : residual(residual), sigmaSquared(sigmaSquared)
  {
  }

  void operator ()(double& pixel, const int* position) const
  {
    const T residualPixel = residual.template at<T>(*position);
    const double residualValueSquared = cv::norm(residualPixel, cv::NORM_L2SQR);
    pixel = sigmaSquared / ((residualValueSquared + sigmaSquared) * (residualValueSquared + sigmaSquared)
    );
    //  pixel = 1 / (2 * sqrtf(residualValueSquared + static_cast<double>(0.1)));
    // pixel = 1 / sqrt(1 + residualValueSquared / lambdaSquared);
  }
};

cv::Mat_<double> IrlsReconstruction::CalculateW(cv::Mat_<double>& residual, double sigmaSquared)
{
  cv::Mat_<double> weights(residual.rows, 1);
  weights.forEach(WCalculatorOperator<double>(residual, sigmaSquared));
  return weights;
}

template <class T>
struct WeightOperator
{
  const cv::Mat_<T>& toWeight;
  cv::Mat_<T>& weighted;

  void operator ()(T& pixel, const int* position) const
  {
    cv::Mat_<T> temp = (pixel * toWeight.row(*position));
    temp.copyTo(weighted.row(*position));
  }
};

cv::Mat_<float> IrlsReconstruction::WeightMatrix(const cv::Mat_<float>& weights, const cv::Mat_<float>& toWeight)
{
  cv::Mat_<float> weighted = cv::Mat::zeros(toWeight.rows, toWeight.cols, toWeight.type());

  weights.forEach(WeightOperator<float>{toWeight, weighted});
  return weighted;
}

cv::Mat_<double> IrlsReconstruction::CalculateInitialWMeanFace(const cv::Mat_<double>& image) const
{
  cv::Mat_<double> meanFace;
  reduce(v, meanFace, 1, cv::REDUCE_SUM, CV_64F);
  meanFace /= v.cols;
  cv::Mat_<double> residual = abs(meanFace - image);
  const double sigmaSquared = CalculateSigmaSquared(residual);

#ifdef NDEBUGDEV
  const auto meanFaceImage = MatUtil::DeVectorizeImage<cv::Vec3d, double>(meanFace.clone(), 3, 95);
  MatUtil::WriteImage("output/MeanFace.png", meanFaceImage);
#endif

  return CalculateW(residual, sigmaSquared);
}

cv::Mat_<double> IrlsReconstruction::CalculateInitialWNearestNeighbour(const cv::Mat_<double>& image) const
{
  std::vector<std::pair<double, int>> diffImageVector;
  for (int i = 0; i < v.cols; i++)
  {
    const cv::Mat_<double> vColumn = v.col(i);
    const double difference1 = norm(vColumn - image, cv::NORM_L1);
    diffImageVector.emplace_back(difference1, i);
  }
  const unsigned int offset = static_cast<unsigned int>(diffImageVector.size() - 1);
  std::nth_element(diffImageVector.begin(), diffImageVector.begin(), diffImageVector.end());
  const cv::Mat_<double> nearestNeighbour = v.col(diffImageVector[0].second);

#ifdef NDEBUGDEV
  const auto diffImageVectorDebug = diffImageVector;
  const auto nearestNeighbourImage = MatUtil::DeVectorizeImage<cv::Vec3d, double>(
    nearestNeighbour.clone(), 3, 95);
  MatUtil::WriteImage("output/NearestNeighbour.png", nearestNeighbourImage);
#endif

  cv::Mat_<double> residual = abs(nearestNeighbour - image);
  const double sigmaSquared = CalculateSigmaSquared(residual);
  return CalculateW(residual, sigmaSquared);
}

cv::Mat_<double> IrlsReconstruction::CalculateInitialWZeroOutLowerPart()
{
  static const int cutoff = 40;
  cv::Mat lowerFaceMask(cutoff, 95, CV_64FC3, cv::Scalar_<double>(1, 1, 1));
  lowerFaceMask.push_back(cv::Mat(95 - cutoff, 95, CV_64FC3, cv::Scalar_<double>(0, 0, 0)));
  return MatUtil::VectorizeImage<double, cv::Vec3d>(lowerFaceMask);
}

cv::Mat_<double> IrlsReconstruction::WeightMatrix(const cv::Mat_<double>& weights, const cv::Mat_<double>& toWeight)
{
  cv::Mat_<double> weighted = cv::Mat::zeros(toWeight.rows, toWeight.cols, toWeight.type());

  weights.forEach(WeightOperator<double>{toWeight, weighted});
  return weighted;
}

void IrlsReconstruction::DebugOutput(const cv::Mat_<double>& reconstructed, const cv::Mat_<double>& cNew,
                                     const cv::Mat_<double>& w, unsigned int i)
{
#ifdef NDEBUGDEV
  auto plot = cv::plot::Plot2d::create(cNew);
  plot->setPlotBackgroundColor({255, 255, 255});

  plot->setPlotGridColor({0, 0, 0});
  plot->setPlotAxisColor({0, 0, 0});
  plot->setPlotLineColor({0, 0, 255});
  plot->setShowText(false);
  plot->setNeedPlotLine(false);
  plot->setShowGrid(false);
  plot->setMaxY(1);
  plot->setMinY(-1);
  cv::Mat plotImage;
  plot->render(plotImage);
  MatUtil::WriteImage("output/coefficients_" + std::to_string(cNew.rows) + "_" + std::to_string(i) + ".png",
                      plotImage);

  MatUtil::WriteVectorizedImage("output/iteration_" + std::to_string(i) + ".png", reconstructed, 3, 95);

  cv::Mat_<double> weightsW;
  normalize(w, weightsW, 0.0, 255.0, cv::NORM_MINMAX);
  cv::Mat_<cv::Vec3d> wImage = MatUtil::DeVectorizeImage<cv::Vec3d, double>(weightsW, 3, 95);
  MatUtil::WriteImage("output/weights" + std::to_string(i) + ".png", wImage);
#endif
}
