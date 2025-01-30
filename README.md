# CompositeRender
A simple Renderer for Composite

## Features
 - Save As Png, Jpg and Bmp
 - Color Balance
 - Color Correction
 - Exposure
 - Alpha Over
 - Gamma

## Goals
 - Generate Image AI Node
 - Remove BG AI Node

## Running
On Windows run:
```
CompositeRenderer.exe <XML File> <Output FilePath>
```

## Building
On Windows, run:
```
cmake --build build --config Release --target ALL_BUILD -j 10 --
```
Make sure you have ninja in your path before using emscripten
For emscripten run:
```
mkdir build.em
cd build.em
emcmake cmake -DEMSCRIPTEN_BUILD=ON .. -G "ninja"
cd ../
emmake ninja
```