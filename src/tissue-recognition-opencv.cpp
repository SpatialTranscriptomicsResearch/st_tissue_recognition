#include <exception>

#include "tissue-recognition-opencv.hpp"
#include "util.hpp"

using namespace cv;
using namespace std;

static void fill_fgd_holes(Mat &msk);
static void remove_small_components(Mat &msk, double threshold);

namespace tr {

void recognize_tissue(const Mat &img, Mat &msk, Mat &user_annotation,
                      const Options &opt) {

  if (msk.size() != img.size()) {
    msk.create(img.rows, img.cols, CV_8UC1);
    init_msk(img, msk);
  }

  if (user_annotation.size() == img.size()) {
    for (auto it = user_annotation.begin<uint8_t>();
         it != user_annotation.end<uint8_t>(); ++it) {
      switch (*it) {
      case Annotation::FGD:
        msk.at<uint8_t>(it.pos()) = GC_FGD;
        break;
      case Annotation::BGD:
        msk.at<uint8_t>(it.pos()) = GC_BGD;
        break;
      }
    }
  }

  Mat bgd_model, fgd_model;
  Rect rect(0, 0, img.cols, img.rows);

  double diff = std::numeric_limits<double>::infinity();
  int i = 0;

  while (diff > opt.threshold_diff && i++ < opt.iteration_max) {
    Mat _msk = msk.clone();
    grabCut(img, msk, rect, bgd_model, fgd_model, 4, GC_INIT_WITH_MASK);
    diff = norm(msk, _msk, NORM_L1);
  }

  remove_small_components(msk, opt.threshold_size);
  fill_fgd_holes(msk);
}

void init_msk(const Mat &img, Mat &msk) {
  // Edge detection
  Canny(img, msk, 10, 200, 3);

  // Remove edges from white- or blackspace margins
  Rect crop_box = get_cropping_box(img);
  resize(crop_box, 0.99);
  for (auto it = msk.begin<uint8_t>(); it != msk.end<uint8_t>(); ++it)
    if (!it.pos().inside(crop_box))
      *it = 0;

  // Blur to fill gaps in edges
  blur(msk, msk, Size(5, 5));

  for (auto it = msk.begin<uint8_t>(); it != msk.end<uint8_t>(); ++it) {
    if (*it == 0)
      *it = GC_PR_BGD;
    else
      *it = GC_PR_FGD;
  }

  fill_fgd_holes(msk);
}

cv::Mat get_binary_mask(const cv::Mat &msk) {
  if (msk.type() != CV_8UC1)
    throw std::invalid_argument("Invalid data type.");
  return (msk == GC_FGD) | (msk == GC_PR_FGD);
}

Mat downsample(const Mat &img, double factor) {
  cv::Mat ret = img.clone();
  scale_down_to_scale(ret, factor);
  return ret;
}
Mat upsample(const Mat &img, double factor) {
  cv::Mat ret = img.clone();
  scale_up_to_scale(ret, factor);
  return ret;
}
}

static void fill_fgd_holes(Mat &msk) {

  uint8_t flag_pr_bgd = 254, flag_bgd = 255;

  convert<uint8_t>(msk, std::map<uint8_t, uint8_t>{{GC_PR_BGD, flag_pr_bgd},
                                                   {GC_BGD, flag_bgd}});

  for (int r = 0, c = 0, dr = 1, dc = 0; c >= 0; r += dr, c += dc) {
    if (c == 0 && r == msk.rows - 1) {
      dr = 0;
      dc = 1;
    }
    if (c == msk.cols - 1 && r == msk.rows - 1) {
      dr = -1;
      dc = 0;
    }
    if (c == msk.cols - 1 && r == 0) {
      dr = 0;
      dc = -1;
    }

    fill<uint8_t>(msk, std::map<uint8_t, uint8_t>{{flag_pr_bgd, GC_PR_BGD},
                                                  {flag_bgd, GC_BGD}},
                  r, c);
  }

  convert<uint8_t>(msk, std::map<uint8_t, uint8_t>{{flag_pr_bgd, GC_PR_FGD},
                                                   {flag_bgd, GC_PR_FGD}});
}

static void remove_small_components(Mat &msk, double threshold) {
  Mat labels = msk.clone();
  convert(labels,
          map<uint8_t, uint8_t>{{GC_PR_BGD, GC_BGD}, {GC_PR_FGD, GC_FGD}});
  connected_components(labels);

  map<int, int> small;
  for (auto it = labels.begin<short>(); it != labels.end<short>(); ++it)
    small[*it]++;

  int pixels_total = msk.cols * msk.rows;
  int nsmall = 0;
  for (auto it = small.begin(); it != small.end(); ++it) {
    if ((double)it->second / pixels_total < threshold) {
      it->second = 1;
      nsmall++;
    } else
      it->second = 0;
  }

  // Scan msk row-wise, copying val of an adjacent large component to the
  // small components
  int large_component = -1;
  vector<uint8_t *> unknown; // Stores small component pixels in case the
                             // first few pixels in msk belong to a
                             // small component
  for (int r = 0, d = 1; r < msk.rows; ++r, d = -d) {
    for (int c = d > 0 ? 0 : msk.cols - 1; c >= 0 && c < msk.cols; c += d) {

      uint8_t &cur_pixel = msk.at<uint8_t>(r, c);
      if (small[labels.at<short>(r, c)]) {
        if (large_component != -1)
          cur_pixel = large_component;
        else
          unknown.push_back(&cur_pixel);
      } else {
        if (large_component == -1)
          for (auto p : unknown)
            *p = cur_pixel;
        large_component = cur_pixel;
      }
    }
  }
}
