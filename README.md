st_tissue_recognition
=====================
st_tissue_recognition is a tool for creating masks around tissue sections in
brightfield images.
It uses the [GrabCut](https://en.wikipedia.org/wiki/GrabCut) algorithm, as
implemented in [OpenCV](http://opencv.org/), along with a few tweaks to guess
the initial segmentation of the image and to remove noise.

The library is written in C++.
There is also a [python extension](#python-usage) that provides python
bindings.

Dependencies
------------
### <a name="dep-main"></a>Main library
```
CMake 2.8+
OpenCV 3.1.0+
```

### <a name="dep-python"></a>Python module
In addition to the [main library](#dep-main),
```
Python 2.7+ *or* Python 3.5+
numpy 1.11.1+
setuptools 20.1.1+
```

### Python module example
In addition to the [python module](#dep-python),
```
Pillow 4.1.0+
scipy 0.19.0+
```

Installation
------------
### Main library
1. Install [OpenCV](https://github.com/opencv/opencv).
2. Run `cmake` on the root directory. For example,

    ```sh
    mkdir build
    cd build
    cmake ..
    make install
    ```

### Python module
1. Install numpy and setuptools using your preferred package manager. For
   example, using pip:

    ```sh
    pip install numpy setuptools
    ```
2. Run setup script:

    ```sh
    cd python-module
    python setup.py install
    ```

Usage
-----
### <a name="python-usage"></a>Python module
See ```./python-module/example.py``` for example usage.

*Note*: Remember to free the return values of ```get_binary_mask```,
    ```downsample```, and ```upsample``` using ```free```.
