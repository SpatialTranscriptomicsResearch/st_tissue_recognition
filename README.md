Installation
------------
### Main library
```
mkdir build
cd build
cmake ..
make install
```

### Python module
```
cd python-module
python3.5 setup.py
```

Usage
-----
### Python module
See ```./python-module/example.py``` for example usage.

Note: Remember to free the return values of ```get_binary_mask```,
```downsample```, and ```upsample``` using ```tissue_recognition.free```.
