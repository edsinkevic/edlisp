#!/bin/sh

BUILD_DIR=cmake-build-debug

cmake . -B $BUILD_DIR
make -C $BUILD_DIR
$BUILD_DIR/edlisp resources/simple-lisp.txt | dot -Tpng -o simple-lisp.png