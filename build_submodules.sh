#!/bin/bash
workdir=$(pwd)
platform=$(uname)
arch=$(uname -m)

# Raylib
if [ ! -f "${workdir}/libs/raylib/lib/libraylib.so" ]; then
  echo 'Building raylib...'
  cd ${workdir}/submodules/raylib
  rm -rf build
  mkdir build && cd build
  cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=${workdir}/libs/raylib ..
  make -j
  make install
fi
echo 'raylib done'

# Raygui
if [ ! -f "${workdir}/libs/raygui/raygui.so" ]; then
  echo 'Building raygui...'
  cd ${workdir}/submodules/raygui
  cp src/raygui.h src/raygui.c
  if [[ $platform == "Darwin" && $arch == "arm64" ]]; then
    gcc -o libraygui.dylib src/raygui.c -shared -fpic -DRAYGUI_IMPLEMENTATION \
        -I${workdir}/libs/raylib/include \
        -L${workdir}/libs/raylib/lib \
        -framework OpenGL -lm -lpthread -ldl $(pkg-config --libs --cflags raylib)
  else
    gcc -o libraygui.so src/raygui.c -shared -fpic -DRAYGUI_IMPLEMENTATION \
        -I${workdir}/libs/raylib/include \
        -L${workdir}/libs/raylib/lib \
        -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
  fi
  if [ ! -d "${workdir}/libs/raygui" ]; then
    mkdir ${workdir}/libs/raygui
  fi
  if [ ! -d "${workdir}/libs/raygui/include" ]; then
    mkdir ${workdir}/libs/raygui/include
  fi
  cp raygui.so ${workdir}/libs/raygui
  if [[ $platform == "Darwin" && $arch == "arm64" ]]; then
    mv ${workdir}/submodules/raygui/libraygui.dylib ${workdir}/libs/raygui/libraygui.dylib
  else
    mv ${workdir}/libs/raygui/libraygui.so ${workdir}/libs/raygui/libraygui.so
  fi
  cp src/raygui.h ${workdir}/libs/raygui/include
fi
echo 'raygui done'

# logger
if [ ! -f "${workdir}/libs/libspdlog.a" ]; then
  echo 'Building logger...'
  cd ${workdir}/submodules/logger
  rm -rf build
  mkdir build && cd build
  cmake .. && make -j
  cp libspdlog.a ${workdir}/libs
fi
echo 'logger done'