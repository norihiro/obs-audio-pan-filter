# OBS Audio Pan Filter

## Introduction

This is a simple plugin to control stereo pan of audio source in OBS Studio.

- Adjust volumes of left and right channel separately
- Adjust pan control of each channel

## Build

### Linux
```
git clone https://github.com/norihiro/obs-command-source.git
cd obs-command-source
mkdir build && cd build
cmake \
	-DLIBOBS_INCLUDE_DIR="<path to the libobs sub-folder in obs-studio's source code>" \
	-DCMAKE_INSTALL_PREFIX=/usr ..
make -j4
sudo make install
```

### OS X
```
git clone https://github.com/norihiro/obs-command-source.git
cd obs-command-source
mkdir build && cd build
cmake \
	-DLIBOBS_INCLUDE_DIR=<path to the libobs sub-folder in obs-studio's source code> \
	-DLIBOBS_LIB=<path to libobs.0.dylib> \
	-DOBS_FRONTEND_LIB=<path to libobs-frontend-api.dylib> \
	..
make -j4
# Copy command-source.so to the obs-plugins folder
```
