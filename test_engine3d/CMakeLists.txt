cmake_minimum_required(VERSION 3.20.0)
project (test_engine3d)

add_executable(test_engine3d test_engine3d.cpp  Engine3D.cpp Engine3D.h Image.h ImageIoBmp.h ImageIoBmp.cpp)

add_executable(test_bmp test_bmp.cpp Image.h ImageIoBmp.h ImageIoBmp.cpp)