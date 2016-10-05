try:
    from .version import __version__
    from .tissue_recognition import recognize_tissue
except ImportError as ierr:
    print("Import error: %s" % ierr)
