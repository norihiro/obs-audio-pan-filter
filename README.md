# OBS Audio Pan Filter

## Introduction

This is a simple plugin to control stereo pan of audio source in OBS Studio.

- Adjust volumes of left and right channel separately
- Adjust pan control of each channel

## Build

### Linux
Make sure `libobsConfig.cmake` is found by cmake.
Then run these commands.
```
git clone https://github.com/norihiro/obs-audio-pan-filter
cd obs-audio-pan-filter
mkdir build && cd build
cmake \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DCMAKE_INSTALL_LIBDIR=/usr/lib \
	..
make -j2
sudo make install
```
You might need to adjust `CMAKE_INSTALL_LIBDIR` for your system.

### macOS
Make sure `libobsConfig.cmake` is found by cmake.
Then run these commands.
```
git clone https://github.com/norihiro/obs-audio-pan-filter
cd obs-audio-pan-filter
mkdir build && cd build
cmake \
	-DLIBOBS_LIB=<path to libobs.0.dylib> \
	..
make -j2
```
Finally, copy obs-audio-pan-filter.so to the obs-plugins folder.
