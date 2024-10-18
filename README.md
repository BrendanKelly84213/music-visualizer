# music-visualizer

Only tested on Ubuntu 22! 

## Install submodules

```
git submodule init
git submodule update
```

## Install dependencies 

```
sudo apt-get install build-essential cmake libglfw3 libglfw3-dev libglm-dev libsdl2-dev libsdl2-mixer-dev libfftw3-dev  libsndfile1-dev
```

## Build

```
mkdir build
cd build
cmake ../
make
```
