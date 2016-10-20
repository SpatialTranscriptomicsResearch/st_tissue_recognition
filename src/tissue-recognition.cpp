#include <map>

#include "tissue-recognition-opencv.hpp"

#include "tissue-recognition.hpp"

// Used to make sure that the memory allocated by tr_get_binary_mask,
// tr_downsample, and tr_upsample doesn't get freed by opencv until tr_free is
// called.
std::map<uchar*, cv::Mat> in_memory;

const std::map<int, int> cv_types{{trImage::UINT8C1, CV_8UC1},
                                  {trImage::UINT8C3, CV_8UC3}};

int tr_recognize_tissue(trImage img, trImage msk, trImage annot,
        bool init_msk, const trOptions& opt) {
    cv::Mat ocv_img(img.rows, img.cols, cv_types.at(img.data_type), img.data);
    cv::Mat ocv_msk(msk.rows, msk.cols, cv_types.at(msk.data_type), msk.data);
    cv::Mat ocv_annot(annot.rows, annot.cols, cv_types.at(annot.data_type),
                      annot.data);

    if(init_msk)
        tr::init_msk(ocv_img, ocv_msk);

    tr::recognize_tissue(ocv_img, ocv_msk, ocv_annot, opt);

    return EXIT_SUCCESS;
}

trImage tr_get_binary_mask(trImage msk) {
    cv::Mat ocv_msk(msk.rows, msk.cols, cv_types.at(msk.data_type), msk.data);
    cv::Mat bin_msk = tr::get_binary_mask(ocv_msk);

    trImage ret{bin_msk.data, trImage::UINT8C1, bin_msk.rows, bin_msk.cols};
    in_memory[ret.data] = bin_msk;

    return ret;
}

trImage tr_downsample(trImage img, double factor) {
    cv::Mat ocv_img(img.rows, img.cols, cv_types.at(img.data_type), img.data);
    cv::Mat res = tr::downsample(ocv_img, factor);

    trImage ret{res.data, trImage::UINT8C3, res.rows, res.cols};
    in_memory[ret.data] = res;

    return ret;
}
trImage tr_upsample(trImage img, double factor) {
    cv::Mat ocv_img(img.rows, img.cols, cv_types.at(img.data_type), img.data);
    cv::Mat res = tr::upsample(ocv_img, factor);

    trImage ret{res.data, trImage::UINT8C3, res.rows, res.cols};
    in_memory[ret.data] = res;

    return ret;
}

int tr_free(trImage img) {
    auto it = in_memory.find(img.data);
    in_memory.erase(it);
    return EXIT_SUCCESS;
}
