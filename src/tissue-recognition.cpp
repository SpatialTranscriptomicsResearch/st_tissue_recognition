#include "tissue-recognition-opencv.hpp"

#include "tissue-recognition.hpp"

void tr_recognize_tissue(uchar *img, uchar *msk, uchar *annot,
        int rows, int cols,
        bool init_msk, const trOptions& opt) {
    cv::Mat ocv_img(rows, cols, CV_8UC3, img);

    cv::Mat ocv_msk(rows, cols, CV_8UC1, msk);
    if(init_msk)
        tr::init_msk(ocv_img, ocv_msk);

    cv::Mat ocv_annot;
    if(annot != NULL)
        ocv_annot = cv::Mat(rows, cols, CV_8UC1, annot);

    tr::recognize_tissue(ocv_img, ocv_msk, ocv_annot, opt);
}

/* FIXME: NOT IMPLEMENTED */
uchar** tr_get_binary_mask(const uchar** msk) {
    return NULL;
}

/* FIXME: NOT IMPLEMENTED */
uchar** tr_downsample(uchar** img, double factor) {
    return NULL;
}
/* FIXME: NOT IMPLEMENTED */
uchar** tr_upsample(uchar** img, double factor) {
    return NULL;
}
