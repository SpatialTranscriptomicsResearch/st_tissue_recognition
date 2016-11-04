import argparse as ap

import numpy as np
from scipy.misc import imread, imshow

import tissue_recognition as tr


def main():
    opt = ap.ArgumentParser()
    opt.add_argument("img_file", type=str)
    opt = opt.parse_args()

    img = imread(opt.img_file)
    msk = np.zeros(img.shape[0:2], dtype=np.uint8)
    tr.recognize_tissue(img, msk)
    bin_msk = tr.get_binary_mask(msk)
    imshow(bin_msk)
    tr.free(bin_msk)

if __name__ == "__main__":
    main()
