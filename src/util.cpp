#include "util.hpp"

using namespace cv;
using namespace std;

size_t compute_color_intensity(const Vec3b &color) {
  return color[0] + color[1] + color[2];
}

// TODO: type checking
// TODO: error handling when there are too many components
void connected_components(cv::Mat &m) {
  // We do a fill on all non-labeled pixels to their label.
  // Labels will be stored as negative values, which allows us to separate
  // between non-labeled and labeled pixels.
  m.convertTo(m, CV_32S);
  int component = 1;
  for (auto it = m.begin<int>(); it != m.end<int>(); ++it)
    if (*it >= 0)
      fill<int>(m, map<int, int>{{*it, -(component++)}}, it.pos().y,
                it.pos().x);
  m = -1 * m - 1;
  m.convertTo(m, CV_16U);
}

Rect get_cropping_box(const Mat &img) {
  Mat _img;
  cvtColor(img, _img, CV_BGR2GRAY);

  vector<Point> inside;
  for (auto it = _img.begin<uint8_t>(); it != _img.end<uint8_t>(); ++it)
    if (*it > 15 && *it < 240)
      inside.push_back(it.pos());

  return inner_bounds(minAreaRect(inside));
}

Rect inner_bounds(cv::RotatedRect rr) {
  Point2f pts[4];
  rr.points(pts);

  int xmax, xmin, ymax, ymin;
  sort(pts, pts + 4,
       [](const Point2f &p1, const Point2f &p2) { return p1.x < p2.x; });
  xmin = pts[1].x;
  xmax = pts[2].x;
  sort(pts, pts + 4,
       [](const Point2f &p1, const Point2f &p2) { return p1.y < p2.y; });
  ymin = pts[1].y;
  ymax = pts[2].y;

  return Rect(xmin, ymin, xmax - xmin, ymax - ymin);
}

void resize(Rect &r, double f) {
  double new_w = r.width * f, new_h = r.height * f;
  r.x = r.x + (r.width - new_w) / 2;
  r.y = r.y + (r.height - new_h) / 2;
  r.height = new_h;
  r.width = new_w;
}

double scale_down_to_max(Mat &img, size_t maxImageSize) {
  double scale = 1.0;
  // if maxImageSize > 0 then images are scaled down (by halving)
  // until the larger dimension is < maxImageSize
  while (maxImageSize > 0 and
         max(static_cast<size_t>(img.rows), static_cast<size_t>(img.cols)) >
             maxImageSize) {
    pyrDown(img, img, Size(img.cols / 2, img.rows / 2));
    scale /= 2;
  }
  return scale;
}

void scale_down_to_scale(Mat &img, double scale) {
  double scale_ = 1.0;
  while (scale_ > scale) {
    pyrDown(img, img, Size(img.cols / 2, img.rows / 2));
    scale_ /= 2;
  }
}

void scale_up_to_scale(Mat &img, double scale) {
  double scale_ = 1.0;
  while (scale_ < scale) {
    pyrUp(img, img, Size(img.cols * 2, img.rows * 2));
    scale_ *= 2;
  }
}
