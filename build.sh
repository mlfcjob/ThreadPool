#!/bin/bash

OUT=ThreadPool
INCLUDE_DIR=`pwd`
INCLUDE_FLAGS=-I $INCLUDE_DIR
LD_FLAGS=-lpthread

g++ -g -o $OUT Thread.cpp main.cpp $INCLUDE_FLAGS $LD_FLAGS
