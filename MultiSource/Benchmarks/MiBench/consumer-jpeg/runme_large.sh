#!/bin/sh
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-encode ${COUNT_STATS} ./jpeg-6a/cjpeg -dct int -progressive -opt -outfile /dev/null input_large.ppm
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-decode ${COUNT_STATS} ./jpeg-6a/djpeg -dct int -ppm -outfile /dev/null input_large.jpg
