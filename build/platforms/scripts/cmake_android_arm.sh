#!/bin/sh
cd `dirname $0`/../../../

mkdir -p build_android_arm
cd build_android_arm

cmake -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON -DCMAKE_TOOLCHAIN_FILE=../build/platforms/android/android.toolchain.cmake -DANDROID_NATIVE_API_LEVEL=android-21 $@ ../
