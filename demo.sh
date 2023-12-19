#!/bin/sh

set -x

BUILD_DIR=cmake-build-debug

cmake . -B $BUILD_DIR
make -C $BUILD_DIR
$BUILD_DIR/edlisp
