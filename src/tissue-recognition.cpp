#include <map>

#include "tissue-recognition-opencv.hpp"

#include "tissue-recognition.hpp"

// Used to make sure that the memory allocated by tr_get_binary_mask,
// tr_downsample, and tr_upsample doesn't get freed by opencv until tr_free is
// called.
std::map<uchar*, cv::Mat> in_memory;

int tr_recognize_tissue(trImage img, trImage msk, trImage annot,
        bool init_msk, const trOptions& opt) {
    cv::Mat ocv_img(img.rows, img.cols, CV_8UC3, img.data);
    cv::Mat ocv_msk(msk.rows, msk.cols, CV_8UC1, msk.data);
    cv::Mat ocv_annot(annot.rows, annot.cols, CV_8UC1, annot.data);

    if(init_msk)
        tr::init_msk(ocv_img, ocv_msk);

    tr::recognize_tissue(ocv_img, ocv_msk, ocv_annot, opt);

    return EXIT_SUCCESS;
}

trImage tr_get_binary_mask(trImage msk) {
    cv::Mat ocv_msk(msk.rows, msk.cols, CV_8UC1, msk.data);
    cv::Mat bin_msk = tr::get_binary_mask(ocv_msk);

    trImage ret{
        .data = bin_msk.data, .rows = bin_msk.rows, .cols = bin_msk.cols};
    in_memory[ret.data] = bin_msk;

    return ret;
}

trImage tr_downsample(trImage img, double factor) {
    cv::Mat ocv_img(img.rows, img.cols, CV_8UC1, img.data);
    cv::Mat res = tr::downsample(ocv_img, factor);

    trImage ret{
        .data = res.data, .rows = res.rows, .cols = res.cols};
    in_memory[ret.data] = res;

    return ret;
}
trImage tr_upsample(trImage img, double factor) {
    cv::Mat ocv_img(img.rows, img.cols, CV_8UC1, img.data);
    cv::Mat res = tr::upsample(ocv_img, factor);

    trImage ret{
        .data = res.data, .rows = res.rows, .cols = res.cols};
    in_memory[ret.data] = res;

    return ret;
}

int tr_free(trImage img) {
    auto it = in_memory.find(img.data);
    in_memory.erase(it);
    return EXIT_SUCCESS;
}
