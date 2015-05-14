#ifndef TISSUE_RECOGNITION_OPTIONS_HPP
#define TISSUE_RECOGNITION_OPTIONS_HPP

#define uchar unsigned char

enum trAnnotation { NA = 0, FGD = 1, BGD = 2 };

typedef struct trOptions {
    int iteration_max = 3;
    double threshold_diff = 0.001,
           threshold_size = 0.001;
} trOptions;

#endif
