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

Troubleshooting
---------------
### Python module import error
**OSError: libtissue-recognition.so: cannot open shared object file: No such
file or directory**.

The linker cannot find the object file of the library.
Thus, you may need to add the dirname of the object file to the search path of
the linker.
If you configured the installation using `cmake ..`, as in the installation
instructions, the object file should be located in `/usr/local/lib`.

If you're using ld/GNU linker, you can try running

```sh
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

and import the module again.
To make the change permanent, either add the above command to the startup script
of your shell (e.g., `~/.bashrc`) or, perhaps better, add the library path to
the ld config file:

```sh
[[ ! -e /etc/ld.so.conf.d/local.conf ]] && \
  echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/local.conf >/dev/null && \
  sudo ldconfig
```
