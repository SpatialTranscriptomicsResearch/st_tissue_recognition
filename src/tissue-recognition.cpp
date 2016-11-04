#include <exception>
#include <map>
#include <string>

#include "tissue-recognition-opencv.hpp"

#include "tissue-recognition.hpp"

// Used to make sure that the memory allocated by tr_get_binary_mask,
// tr_downsample, and tr_upsample doesn't get freed by opencv until tr_free is
// called.
std::map<uchar *, cv::Mat> in_memory;

const std::map<int, int> cv_types{{trImage::UINT8C1, CV_8UC1},
                                  {trImage::UINT8C3, CV_8UC3}};

static cv::Mat to_mat(trImage im) {
  if (im.data == NULL)
    return cv::Mat();
  return cv::Mat(im.rows, im.cols, cv_types.at(im.data_type), im.data);
}

int tr_recognize_tissue(trImage img, trImage msk, trImage ann, bool init_msk,
                        const trOptions &opt) {
  if (msk.data == NULL)
      throw std::invalid_argument("Mask data is not allowed to be NULL.");

  std::map<std::string, trImage &> ims{
      {"img", img}, {"msk", msk}, {"ann", ann}};

  int rows = img.rows, cols = img.cols;
  for (auto &kv : ims)
    if (kv.second.data != NULL &&
        (kv.second.cols != cols || kv.second.rows != rows))
      throw std::invalid_argument("Images do not have the same dimensions.");

  std::map<std::string, cv::Mat> cv_ims;
  for (auto &kv : ims)
    cv_ims[kv.first] = to_mat(kv.second);

  if (init_msk)
    tr::init_msk(cv_ims["img"], cv_ims["msk"]);

  tr::recognize_tissue(cv_ims["img"], cv_ims["msk"], cv_ims["ann"], opt);

  return EXIT_SUCCESS;
}

trImage tr_get_binary_mask(trImage msk) {
  cv::Mat cv_msk = to_mat(msk);
  cv::Mat bin_msk = tr::get_binary_mask(cv_msk);

  trImage ret{bin_msk.data, trImage::UINT8C1, bin_msk.rows, bin_msk.cols};
  in_memory[ret.data] = bin_msk;

  return ret;
}

trImage tr_downsample(trImage img, double factor) {
  cv::Mat cv_img = to_mat(img);
  cv::Mat res = tr::downsample(cv_img, factor);

  trImage ret{res.data, trImage::UINT8C3, res.rows, res.cols};
  in_memory[ret.data] = res;

  return ret;
}
trImage tr_upsample(trImage img, double factor) {
  cv::Mat cv_img = to_mat(img);
  cv::Mat res = tr::upsample(cv_img, factor);

  trImage ret{res.data, trImage::UINT8C3, res.rows, res.cols};
  in_memory[ret.data] = res;

  return ret;
}

int tr_free(trImage img) {
  auto it = in_memory.find(img.data);
  in_memory.erase(it);
  return EXIT_SUCCESS;
}
