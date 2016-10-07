import ctypes as ct

import numpy as np

from . import utils


LIB_FILE = ct.util.find_library("tissue-recognition")
LIB_HANDLE = ct.cdll.LoadLibrary(LIB_FILE)

if LIB_HANDLE is None:
    raise "Unable to import libtissue-recognition.so"

DEF_MAX_ITER = utils.get_val(LIB_HANDLE.TR_DEF_MAX_ITERATIONS, ct.c_int)
DEF_SIZE_THRESHOLD = utils.get_val(LIB_HANDLE.TR_DEF_SIZE_THRESHOLD,
                                   ct.c_double)
DEF_DIFF_THRESHOLD = utils.get_val(LIB_HANDLE.TR_DEF_DIFF_THRESHOLD,
                                   ct.c_double)

FUN = LIB_HANDLE.tr_recognize_tissue


class Options(ct.Structure):
    _fields_ = [("iteration_max", ct.c_int),
                ("threshold_diff", ct.c_double),
                ("threshold_size", ct.c_double)]

    def __init__(self, max_iter=DEF_MAX_ITER,
                 diff_threshold=DEF_DIFF_THRESHOLD,
                 size_threshold=DEF_SIZE_THRESHOLD):
        super(Options, self).__init__(max_iter, diff_threshold, size_threshold)

FUN.argtypes = [np.ctypeslib.ndpointer(ct.c_uint8, flags="C_CONTIGUOUS"),
                np.ctypeslib.ndpointer(ct.c_uint8, flags="C_CONTIGUOUS"),
                np.ctypeslib.ndpointer(ct.c_uint8, flags="C_CONTIGUOUS"),
                ct.c_int,
                ct.c_int,
                ct.c_bool,
                ct.POINTER(Options)]
FUN.restype = None


def recognize_tissue(img, mask, annotations, init_mask=False, options=None):
    if options is None:
        options = Options()

    img, mask, annotations = [
        im.astype(np.uint8) for im in (img, mask, annotations)
        ]

    FUN(img, mask, annotations, mask.shape[0], mask.shape[1],
        init_mask, ct.byref(options))

    return mask
