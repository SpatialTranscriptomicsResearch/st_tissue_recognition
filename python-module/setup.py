"""
setup.py
"""
import re
import subprocess as sp
import setuptools as st

try:
    VERSION = sp.check_output(["git", "describe"]).decode("utf8").strip()
    # Make VERSION compliant to PEP440
    VERSION = re.compile(r"([^\-]+?)-([^\-]+?)-(.+?)").sub(
        r"\g<1>.post\g<2>+\g<3>", VERSION)
except sp.CalledProcessError:
    VERSION = "?"

st.setup(
    name="tissue_recognition",
    packages=['tissue_recognition'],
    version=VERSION,
    zip_safe=True
)
