#ifndef TISSUE_RECOGNITION_OPTIONS_HPP
#define TISSUE_RECOGNITION_OPTIONS_HPP

#define uchar unsigned char

extern const int TR_DEF_MAX_ITERATIONS = 3;
extern const double TR_DEF_DIFF_THRESHOLD = 0.001;
extern const double TR_DEF_SIZE_THRESHOLD = 0.001;

enum trAnnotation { NA = 0, FGD = 1, BGD = 2 };

typedef struct trOptions {
    int iteration_max = TR_DEF_MAX_ITERATIONS;
    double threshold_diff = TR_DEF_DIFF_THRESHOLD,
           threshold_size = TR_DEF_SIZE_THRESHOLD;
} trOptions;

#endif
