#ifndef TISSUE_RECOGNITION_OPENCV_HPP
#define TISSUE_RECOGNITION_OPENCV_HPP

#include <opencv2/opencv.hpp>

#include "tissue-recognition-options.hpp"

namespace tr {

typedef trAnnotation Annotation;
typedef trOptions Options;

void recognize_tissue(const cv::Mat &img, cv::Mat &mask,
                      cv::Mat &user_annotation, const Options &opt = Options());

void init_msk(const cv::Mat &img, cv::Mat &msk);

cv::Mat get_binary_mask(const cv::Mat &mask);

cv::Mat downsample(const cv::Mat &img, double factor);
cv::Mat upsample(const cv::Mat &img, double factor);
}

#endif
