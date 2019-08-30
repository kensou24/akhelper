#include "alg.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <QMutex>
#include <QMutexLocker>


using namespace cv;
using namespace cv::dnn;

extern cv::Mat g_out;
extern QMutex  g_outMutext;

alg::alg()
{}

void alg::fillFeature2D(QString                 algName,
                        cv::Ptr<cv::Feature2D>& algorithm)
{
  using namespace cv;
  using namespace std;

  if (algName == "AGAST")
  {
    Ptr<AgastFeatureDetector> agast = AgastFeatureDetector::create();
    agast->setThreshold(10);
    agast->setNonmaxSuppression(false);
    agast->setType(0);
    algorithm = agast;
  } else if (algName == "KAZE")
  {
    Ptr<AKAZE> akaze = AKAZE::create();
    akaze->setDescriptorChannels(3);
    akaze->setDescriptorSize(0);
    akaze->setDescriptorType(0 + 2);
    akaze->setDiffusivity(0);
    akaze->setNOctaves(4);
    akaze->setNOctaveLayers(4);
    akaze->setThreshold(0.0001);
    algorithm = akaze;
  } else if (algName == "ORB")
  {
    Ptr<ORB> orb = ORB::create();
    orb->setMaxFeatures(500);
    orb->setScaleFactor(1.2);
    orb->setNLevels(8);
    orb->setPatchSize(31);
    orb->setEdgeThreshold(31); // same as patch size
    orb->setWTA_K(2);
    orb->setScoreType(ORB::FAST_SCORE);
    orb->setPatchSize(31);
    orb->setFastThreshold(20);
    algorithm = orb;
  }
}

cv::Point2f alg::processImage(const cv::Mat& inputImage,
                              cv::Mat      & secondImage,
                              cv::Mat      & outputImage,
                              QString        algTypeStr,
                              int            algTypeData,
                              bool           outImg)
{
  using namespace cv;
  using namespace std;
  Point2f middle(0, 0);

  if (secondImage.empty())
  {
    qDebug() << ("Select a secondary image first!");
    return middle;
  }


  Ptr<Feature2D> keypDetector;
  Ptr<Feature2D> descExtractor;
  Ptr<DescriptorMatcher> descMather;
  vector<KeyPoint> keypoints1, keypoints2;
  Mat descriptor1, descriptor2;
  vector<DMatch> matches;

  qDebug() << "使用算法" << algTypeStr;

  try {
    if (algTypeData == 0) {
      fillFeature2D("KAZE", keypDetector);
      fillFeature2D("KAZE", descExtractor);

      descExtractor->detectAndCompute(inputImage,  Mat(), keypoints1,
                                      descriptor1);
      descExtractor->detectAndCompute(secondImage, Mat(), keypoints2,
                                      descriptor2);

      // Also check for the correct type of matching
      descMather = DescriptorMatcher::create(0 + 1);
    } else if (algTypeData == 1) {
      fillFeature2D("AGAST", keypDetector);
      fillFeature2D("ORB",   descExtractor);
      keypDetector->detect(inputImage, keypoints1);
      descExtractor->compute(inputImage, keypoints1, descriptor1);
      keypDetector->detect(secondImage, keypoints2);
      descExtractor->compute(secondImage, keypoints2, descriptor2);

      // Also check for the correct type of matching
      descMather = DescriptorMatcher::create(3 + 1);
    } else {
      qDebug() << "尚不支持的算法";
      return middle;
    }

    descMather->match(descriptor1, descriptor2, matches);

    // Find good matches (AKAZE)
    vector<DMatch> goodMatches;
    double matchThresh = 0.1;

    for (int i = 0; i < descriptor1.rows; i++)
    {
      if (matches[i].distance < matchThresh) goodMatches.push_back(matches[i]);
    }

    QString info;

    for (int i = 0; i < goodMatches.size(); i++) {
      info.append(QString(
                    "keypoints1 index %1 imgIdx=%2 queryIdx=%3 trainIdx=%4").arg(
                    QString::number(
                      i)).arg(
                    QString::
                    number(
                      goodMatches
                      [
                        i
                      ]
                      .imgIdx))
                  .arg(QString::number(goodMatches[i].queryIdx))
                  .arg(QString::number(goodMatches[i].trainIdx))).append("\n ");
    }

    qDebug() << "info data " << info;


    if (outImg) {
      drawMatches(inputImage,
                  keypoints1,
                  secondImage,
                  keypoints2,
                  goodMatches,
                  outputImage);
    }

    vector<Point2f> goodP1, goodP2;

    if (goodMatches.size() == 0) {
      qDebug() << "没有找到数据项";
      return middle;
    }


    for (size_t i = 0; i < goodMatches.size(); i++)
    {
      goodP1.push_back(keypoints1[goodMatches[i].queryIdx].pt);
      goodP2.push_back(keypoints2[goodMatches[i].trainIdx].pt);
    }
    Mat homoChange = findHomography(goodP1, goodP2);

    vector<Point2f> corners1(4), corners2(4);
    corners1[0] = Point2f(0, 0);
    corners1[1] = Point2f(inputImage.cols - 1, 0);
    corners1[2] = Point2f(inputImage.cols - 1, inputImage.rows - 1);
    corners1[3] = Point2f(0, inputImage.rows - 1);

    perspectiveTransform(corners1, corners2, homoChange);

    if (outImg) {
      Point2f offset(inputImage.cols, 0);

      line(outputImage,
           corners2[0] + offset,
           corners2[1] + offset,
           Scalar::all(255),
           8);
      line(outputImage,
           corners2[1] + offset,
           corners2[2] + offset,
           Scalar::all(255),
           8);
      line(outputImage,
           corners2[2] + offset,
           corners2[3] + offset,
           Scalar::all(255),
           8);
      line(outputImage,
           corners2[3] + offset,
           corners2[0] + offset,
           Scalar::all(255),
           8);

      {
        QMutexLocker locker(&g_outMutext);
        g_out = (outputImage.clone());
      }
    }

    for (int i = 0; i < 4; i++) {
      qDebug() << "查找的结果是 i" << i << " " << QString::number(corners2[i].x) <<
        ", " << QString::number(corners2[i].y);
    }


    /**中间点数据*/
    middle = (corners2[0] + corners2[2]) / 2;
    qDebug() << "中间点" << middle.x << "," << middle.y;
  } catch (exception e) {}
  return middle;
}
