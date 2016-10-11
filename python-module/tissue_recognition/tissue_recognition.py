import ctypes as ct

import numpy as np

from . import utils


LIB_FILE = ct.util.find_library("tissue-recognition")
LIB_HANDLE = ct.cdll.LoadLibrary(LIB_FILE)

if LIB_HANDLE is None:
    raise ImportError("Unable to import libtissue-recognition.so")

DEF_MAX_ITER = utils.get_val(LIB_HANDLE.TR_DEF_MAX_ITERATIONS, ct.c_int)
DEF_SIZE_THRESHOLD = utils.get_val(LIB_HANDLE.TR_DEF_SIZE_THRESHOLD,
                                   ct.c_double)
DEF_DIFF_THRESHOLD = utils.get_val(LIB_HANDLE.TR_DEF_DIFF_THRESHOLD,
                                   ct.c_double)


class Image(ct.Structure):
    """Corresponds to the trImage struct in the c-header"""

    _fields_ = [("data", ct.POINTER(ct.c_uint8)),
                ("rows", ct.c_int),
                ("cols", ct.c_int)]

    def __init__(self, img):
        """Initializes object from SciPy image"""

        if img.dtype != np.dtype("uint8"):
            raise TypeError("Image must be uint8 (is {})".format(img.dtype))
        if not img.data.c_contiguous:
            raise ValueError("Image data is non-contiguous")

        super(Image, self).__init__(
            img.ctypes.data_as(ct.POINTER(ct.c_uint8)),
            img.shape[0], img.shape[1])


class Options(ct.Structure):
    """Corresponds to the trOptions struct in the c-header"""

    _fields_ = [("iteration_max", ct.c_int),
                ("threshold_diff", ct.c_double),
                ("threshold_size", ct.c_double)]

    def __init__(self, max_iter=DEF_MAX_ITER,
                 diff_threshold=DEF_DIFF_THRESHOLD,
                 size_threshold=DEF_SIZE_THRESHOLD):
        super(Options, self).__init__(max_iter, diff_threshold, size_threshold)


RECOGNIZE = LIB_HANDLE.tr_recognize_tissue

RECOGNIZE.argtypes = [Image, Image, Image, ct.c_bool, ct.POINTER(Options)]
RECOGNIZE.restype = None


def recognize_tissue(img, mask, annotations, init_mask=True, options=None):
    """
    Runs the recognize_tissue method declared in the c-header.

    Arguments:
    ----------
    `img`:         SciPy array (the image of the tissue section)
    `mask`:        SciPy array
    `annotations`: SciPy array
    `init_mask`:   Boolean. If true, means that the mask will be initialized
                   using the tr::init_mask method from the c++ library. Should
                   be set to False if rerunning with non-empty mask.
    `options`:     See the Options class.

    Notes:
    ------
    - `mask` and `annotations` must have the same dimensions as `img`.
    """
    if options is None:
        options = Options()

    dims = [im.shape[0:2] for im in (img, mask, annotations)]
    if [d[1:] == d[:-1] for d in zip(*dims)] != [1, 1]:
        raise ValueError("Images must have the same dimensions "
                         "(was given images with dimensions "
                         "{}, {}, and {})".format(*dims))

    img, mask, annotations = [
        Image(im) for im in (img, mask, annotations)
        ]

    RECOGNIZE(img, mask, annotations, init_mask, ct.byref(options))

    return mask


# TODO: Implement interfaces for get_binary_mask and down-/upsample
