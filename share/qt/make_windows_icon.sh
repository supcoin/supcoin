#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/supcoin.png
ICON_DST=../../src/qt/res/icons/supcoin.ico
convert ${ICON_SRC} -resize 16x16 supcoin-16.png
convert ${ICON_SRC} -resize 32x32 supcoin-32.png
convert ${ICON_SRC} -resize 48x48 supcoin-48.png
convert supcoin-16.png supcoin-32.png supcoin-48.png ${ICON_DST}

