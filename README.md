# YATRT
Yet Another Tiny Ray Tracer

A C implementation of the famous [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) book.

Currently, it is aimed for RP2040 Pi Pico W, with a TFT 128x160 SPI TFT Display.

## Goals
* Run the full ray tracing loop on target.
* Enable dual core support.
* Enable DMA on Display copy.
* Avoid Dynamic mem. allocation.
* Simplify/ Accelerate some of the RT aspects.

# Requirements 
- CMake
- GCC 12 (arm-none-eabi)
- Pico-SDK