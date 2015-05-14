#ifndef TISSUE_RECOGNITION_HPP
#define TISSUE_RECOGNITION_HPP

#define uchar unsigned char

#include "tissue-recognition-options.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void tr_recognize_tissue(uchar *img, uchar *msk, uchar *annotations,
        int rows, int cols,
        bool init_msk = false, const trOptions &opt = trOptions());

uchar** tr_get_binary_mask(const uchar **mask);

uchar** tr_downsample(uchar **img, double factor);
uchar** tr_upsample(uchar **img, double factor);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
