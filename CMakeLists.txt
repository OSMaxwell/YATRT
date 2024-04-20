cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(pico_sdk_import.cmake)

project(yatrt)
pico_sdk_init()
set(EXE_NAME pico-yatrt)
# library configuration (override if necessary, see pico-st7735 for a complete list)
set(SPI_TFT_PORT "spi1" CACHE STRING "TFT spi port-name")
set(SPI_TFT_CS   "9"    CACHE STRING "TFT CS  pin number")
set(SPI_TFT_DC   "10"   CACHE STRING "TFT DC  pin number")
set(SPI_TFT_RST  "11"   CACHE STRING "TFT RST pin number")
set(TFT_OPTIONS TFT_ENABLE_RED TFT_ENABLE_RESET TFT_ENABLE_TEXT TFT_ENABLE_SHAPES
                TFT_ENABLE_ROTATE TFT_ENABLE_SCROLL TFT_ENABLE_FONTS
            CACHE STRING "TFT options/functions")

#program configuration
set(SPI_PORT "spi1" CACHE STRING "spi port-name")
set(SPI_RX   "12"   CACHE STRING "RX  pin number")
set(SPI_TX   "15"   CACHE STRING "TX  pin number")
set(SPI_SCK  "14"   CACHE STRING "SCK pin number")

add_compile_definitions(SPI_PORT=${SPI_PORT})
add_compile_definitions(SPI_RX=${SPI_RX})
add_compile_definitions(SPI_TX=${SPI_TX})
add_compile_definitions(SPI_SCK=${SPI_SCK})

foreach(opt IN LISTS TFT_OPTIONS)
    add_compile_definitions(${opt})
endforeach(opt)

# add preprocessor-constant DEBUG for Debug-builds
#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#  set(CMAKE_VERBOSE_MAKEFILE 1)
#  add_compile_definitions(DEBUG)
#else()
#endif()

include_directories(include/common)

# Custom-made libs
add_library(ColorLib src/color.c)
target_include_directories(ColorLib PUBLIC include/color 
                        extern/pico-st7735/lib-st7735/include)
target_link_libraries(ColorLib Vec3Lib)

add_library(CameraLib src/camera.c) 
target_include_directories(CameraLib PUBLIC include/camera) 
target_link_libraries(CameraLib ColorLib Vec3Lib HittableLib MaterialLib)

add_library(MaterialLib src/material.c)
target_include_directories(MaterialLib PUBLIC include/material)
target_link_libraries(MaterialLib ColorLib HittableLib RayLib)

add_library(Vec3Lib src/vec3.c)
target_include_directories(Vec3Lib PUBLIC include/vec3)

add_library(SphereLib src/sphere.c)
target_include_directories(SphereLib PUBLIC include/sphere)
target_link_libraries(SphereLib HittableLib MaterialLib)

add_library(RayLib src/ray.c)
target_include_directories(RayLib PUBLIC include/ray)
target_link_libraries(RayLib Vec3Lib)

add_library(HittableLib src/hittable.c src/hittable_list.c)
target_include_directories(HittableLib PUBLIC include/hittable)
target_link_libraries(HittableLib RayLib MaterialLib Vec3Lib)


add_executable(${EXE_NAME} src/main.c)
target_compile_options(${EXE_NAME} PRIVATE -O3)
target_compile_definitions(${EXE_NAME} PUBLIC 
    USE_PICO_RAND) # use pico_rand instead of stdlib

add_subdirectory(extern/pico-st7735)

target_link_libraries(${EXE_NAME}
                    pico_stdlib
                    pico_multicore
                    pico_rand
                    hardware_spi
                    st7735)

target_link_libraries(${EXE_NAME}
                    CameraLib
                    SphereLib)

pico_enable_stdio_uart(${EXE_NAME} 0)
pico_enable_stdio_usb(${EXE_NAME} 1)

pico_add_extra_outputs(${EXE_NAME})