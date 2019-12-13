#include "ibspch.h"
#include "MatrixUtil.h"

const double retain = 0.95;

int main(int argc, char* argv[])
{
  std::filesystem::create_directories("./results/");

  try
  {
    const cv::Mat_<double> beards = MatUtil::ReadMatrixFromFile("./matrix.yml");
    cv::PCA pca(beards, cv::Mat_<double>(), cv::PCA::DATA_AS_COL, retain);
    std::cout << "PCA finished\n";

    cv::FileStorage meanBeard("./results/meanBeard.yml", cv::FileStorage::WRITE);
    meanBeard << "NoBeardMat" << pca.mean;
    meanBeard.release();

    cv::FileStorage pcs("./results/beardPcs.yml", cv::FileStorage::WRITE);
    pcs << "NoBeardMat" << pca.eigenvectors.t();
    pcs.release();
  }
  catch (cv::Exception& e)
  {
    std::cout << e.msg << std::endl;
  }
}
