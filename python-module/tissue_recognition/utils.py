import ctypes as ct


def get_val(ptr, ptr_type):
    return ct.cast(ptr, ct.POINTER(ptr_type)).contents.value
