from setuptools import setup
from Cython.Build import cythonize
import os

os.environ["CYTHON_EMBED"] = "1"

setup(
    ext_modules=cythonize("ex.pyx", compiler_directives={'language_level': "3"}),
)
