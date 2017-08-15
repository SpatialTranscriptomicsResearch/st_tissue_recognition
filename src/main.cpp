#include <chrono>
#include <cstring>
#include <queue>
#include <thread>
#include <vector>

#include <opencv2/opencv.hpp>

#include "git_config.hpp"

#include "tissue-recognition-opencv.hpp"

using namespace cv;
using namespace std;
using namespace tr;

static void help() {
  cout << "This program tries to infer tissue and surrounding in images of "
          "HE-stained tissues on micro-arrays.\n"
          "Usage:\n"
          "tissue-recognition <image_name> [<scale>], default is scale = "
          "1.0\n\n"
          "Version: " GIT_DESCRIPTION
       << endl;
}

static const unsigned short radius = 10;
static const map<int, Scalar> colors{{Annotation::FGD, Scalar(0, 255, 0)},
                                     {Annotation::BGD, Scalar(0, 0, 255)}};

typedef struct Drawing {
  Drawing(string s, Mat &img) : name(s) {
    out = img.clone();
    annotations.create(out.size(), CV_8UC1);
  }
  void show() { imshow(name, out); }

  string name;
  Mat out, annotations;
  bool active = false;
  vector<pair<Point, int>> points;
} Drawing;

static void draw_point(Point p, int type, Drawing *drawing);
static void redraw(Mat &img, Mat &mask, Drawing *drawing);

static void mouse_callback(int event, int c, int r, int flags, void *status);

int main(int argc, char **argv) {
  if (argc < 2 or argc > 3) {
    help();
    return EXIT_FAILURE;
  }

  string filename = argv[1];

  double scale = 1.0;
  if (argc == 3) {
    scale = atof(argv[2]);
  }
  Mat img = imread(filename), mask;
  if (img.empty()) {
    help();
    cout << "can not open " << filename << endl;
    return EXIT_FAILURE;
  }

  std::cout << "downsampling..." << std::endl;
  int orig_cols = img.cols, orig_rows = img.rows;
  img = tr::downsample(img, scale);
  std::cout << "done. (resized from " << orig_cols << "x" << orig_rows << " to "
            << img.cols << "x" << img.rows << ".)" << std::endl;

  Drawing drawing("tissue-recognition", img);

  namedWindow(drawing.name, WINDOW_AUTOSIZE);
  setMouseCallback(drawing.name, mouse_callback, &drawing);

  for (char c = 'n';; c = waitKey(0)) {
    switch (c) {
    case 'n':
      cout << "running recognize_tissue..." << endl;
      recognize_tissue(img, mask, drawing.annotations);
      cout << "done." << endl;
      // fall through
    case 'c':
      drawing.points.clear();
      redraw(img, mask, &drawing);
      break;
    case 's': {
      cout << "upsampling and binarizing mask..." << endl;
      Mat _mask = get_binary_mask(tr::upsample(mask, 1 / scale));
      string filename_out = "mask_" + filename;
      cout << "writing mask to " << filename_out << endl;
      imwrite(filename_out, _mask);
      cout << "done." << endl;
    } break;
    case 'q':
      cout << "exiting." << endl;
      goto main_end;
    }
  }

main_end:
  return EXIT_SUCCESS;
}

static void draw_point(Point p, int type, Drawing *drawing) {
  circle(drawing->annotations, p, radius, type, -1);
  circle(drawing->out, p, radius, colors.at(type), -1);
}

static void redraw(Mat &img, Mat &mask, Drawing *drawing) {
  drawing->out.release();
  img.copyTo(drawing->out, get_binary_mask(mask));
  addWeighted(img, 0.3, drawing->out, 0.7, 0, drawing->out);
  for (auto &p : drawing->points)
    draw_point(p.first, p.second, drawing);
  drawing->show();
}

static void mouse_callback(int event, int c, int r, int flags, void *_drawing) {

  Drawing *drawing = (Drawing *)_drawing;

  switch (event) {
  case EVENT_LBUTTONDOWN:
    drawing->active = true;
    // fall through

  case EVENT_MOUSEMOVE:
    if (drawing->active) {

      int type;
      if (flags & EVENT_FLAG_CTRLKEY)
        type = Annotation::BGD;
      else
        type = Annotation::FGD;

      Point p(c, r);

      drawing->points.push_back(make_pair(p, type));
      draw_point(p, type, drawing);

      drawing->show();
    }
    break;

  case EVENT_LBUTTONUP:
    drawing->active = false;
    break;
  }
}
