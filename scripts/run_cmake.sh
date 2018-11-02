#!/bin/sh

PWD=$(PWD)
SCRIPTSDIR=$PWD/scripts
mkdir -p build
cd build
sh $SCRIPTSDIR/clean.sh

cmake -DPYTHON_LIBRARY=/usr/local/Cellar/python3/3.7.0/Frameworks/Python.framework/Versions/3.7/lib/libpython3.7m.dylib \
	  -DPYTHON_INCLUDE_DIR=/usr/local/Cellar/python/3.7.0/Frameworks/Python.framework/Versions/3.7/include/python3.7m \
	  -DBOOST_ROOT=/usr/local/Cellar/boost/1.67.0_1 \
      -DBOOST_INCLUDEDIR=/usr/local/Cellar/boost/1.67.0_1/include \
	  -DBOOST_LIBRARYDIR=/usr/local/Cellar/boost-python3/1.67.0_1/lib \
	  ..

make

cd ..
