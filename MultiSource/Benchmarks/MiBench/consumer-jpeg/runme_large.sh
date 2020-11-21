#!/bin/sh
# TODO: STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-encode ${COUNT_STATS} ./consumer-jpeg-encode -dct int -progressive -opt -outfile /dev/null input_large.ppm
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-decode ${COUNT_STATS} ./consumer-jpeg-decode -dct int -ppm -outfile /dev/null input_large.jpg
