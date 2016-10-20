#ifndef TISSUE_RECOGNITION_HPP
#define TISSUE_RECOGNITION_HPP

#include "tissue-recognition-options.hpp"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct trImage {
    uchar *data;
    int rows, cols;
} trImage;

int tr_recognize_tissue(trImage img, trImage msk, trImage annotations,
        bool init_msk = false, const trOptions &opt = trOptions());

trImage tr_get_binary_mask(trImage mask);

trImage tr_downsample(trImage img, double factor);
trImage tr_upsample(trImage img, double factor);

int tr_free(trImage);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
