#!/bin/bash
workdir=$(pwd)

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
  gcc -o raygui.so src/raygui.c -shared -fpic -DRAYGUI_IMPLEMENTATION \
      -I${workdir}/libs/raylib/include \
      -L${workdir}/libs/raylib/lib \
      -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
  if [ ! -d "${workdir}/libs/raygui" ]; then
    mkdir ${workdir}/libs/raygui
  fi
  if [ ! -d "${workdir}/libs/raygui/include" ]; then
    mkdir ${workdir}/libs/raygui/include
  fi
  cp raygui.so ${workdir}/libs/raygui
  mv ${workdir}/libs/raygui/raygui.so ${workdir}/libs/raygui/libraygui.so
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