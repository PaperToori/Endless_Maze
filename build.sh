#!/bin/sh
clang++ main.cpp -Wall -std=c++11 `pkg-config --libs --cflags raylib` -o maze