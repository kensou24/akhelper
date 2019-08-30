#ifndef ALG_H
#define ALG_H

#include <QtCore>

#include "opencv2/opencv.hpp"

class alg {
public:

  alg();
  cv::Point2f processImage(const cv::Mat& inputImage,
                           cv::Mat      & secondImage,
                           cv::Mat      & outputImage,

                           QString        algTypeStr,
                           int            algTypeData,
                           bool           outImg = false);
  void fillFeature2D(QString                 algName,
                     cv::Ptr<cv::Feature2D>& algorithm);
};

Q_GLOBAL_STATIC(alg,
                g_alg)
#endif // ALG_H
