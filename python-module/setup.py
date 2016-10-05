"""
setup.py
"""
import setuptools as st

from layer_reg.version import __version__

st.setup(
    name="tissue_recognition",
    packages=['tissue_recognition'],
    version=__version__,
    zip_safe=True
)
