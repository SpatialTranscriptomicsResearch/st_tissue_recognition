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

*Note*: Tested with OpenCV 3.1.0

### Python module
1. Install numpy and setuptools. For example, using pip:

    ```sh
    pip install numpy setuptools
    ```
2. Run setup script:

    ```sh
    cd python-module
    python setup.py install
    ```

*Note*: Tested with Python 2.7 and 3.5, numpy 1.11.1, and setuptools 20.1.1.

Usage
-----
### Python module
See ```./python-module/example.py``` for example usage.

*Note*: Remember to free the return values of ```get_binary_mask```,
    ```downsample```, and ```upsample``` using ```free```.
