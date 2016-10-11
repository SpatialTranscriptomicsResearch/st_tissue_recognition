#include "tissue-recognition-opencv.hpp"

#include "tissue-recognition.hpp"

void tr_recognize_tissue(trImage img, trImage msk, trImage annot,
        bool init_msk, const trOptions& opt) {
    cv::Mat ocv_img(img.rows, img.cols, CV_8UC3, img.data);
    cv::Mat ocv_msk(msk.rows, msk.cols, CV_8UC1, msk.data);
    cv::Mat ocv_annot(annot.rows, annot.cols, CV_8UC1, annot.data);

    if(init_msk)
        tr::init_msk(ocv_img, ocv_msk);

    tr::recognize_tissue(ocv_img, ocv_msk, ocv_annot, opt);
}

trImage tr_get_binary_mask(trImage msk) {
    cv::Mat ocv_msk(msk.rows, msk.cols, CV_8UC1, msk.data);
    cv::Mat bin_msk = tr::get_binary_mask(ocv_msk);

    return trImage{
        .data = bin_msk.data, .rows = bin_msk.rows, .cols = bin_msk.cols};
}

trImage tr_downsample(trImage img, double factor) {
    cv::Mat ocv_img(img.rows, img.cols, CV_8UC1, img.data);
    cv::Mat res = tr::downsample(ocv_img, factor);

    return trImage{.data = res.data, .rows = res.rows, .cols = res.cols};
}
trImage tr_upsample(trImage img, double factor) {
    cv::Mat ocv_img(img.rows, img.cols, CV_8UC1, img.data);
    cv::Mat res = tr::upsample(ocv_img, factor);

    return trImage{.data = res.data, .rows = res.rows, .cols = res.cols};
}
