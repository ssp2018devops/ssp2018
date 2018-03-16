cmake_minimum_required(VERSION 3.2)

project(sdl2_image C)

set(SRC_DIR "sdl2_image")

add_library(sdl2_image SHARED ${SRC_DIR}/IMG.c
${SRC_DIR}/IMG_bmp.c
${SRC_DIR}/IMG_gif.c
${SRC_DIR}/IMG_jpg.c
${SRC_DIR}/IMG_lbm.c
${SRC_DIR}/IMG_pcx.c
${SRC_DIR}/IMG_png.c
${SRC_DIR}/IMG_pnm.c
${SRC_DIR}/IMG_svg.c
${SRC_DIR}/IMG_tga.c
${SRC_DIR}/IMG_tif.c
${SRC_DIR}/IMG_webp.c
${SRC_DIR}/IMG_WIC.c
${SRC_DIR}/IMG_xcf.c
${SRC_DIR}/IMG_xpm.c
${SRC_DIR}/IMG_xv.c
${SRC_DIR}/IMG_xxx.c)



target_link_libraries(sdl2_image SDL2)
target_include_directories(sdl2_image PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}>)
