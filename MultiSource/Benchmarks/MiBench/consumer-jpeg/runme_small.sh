#!/bin/sh
# STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-encode ${COUNT_STATS} ./jconsumer-jpeg-encode -dct int -progressive -opt -outfile /dev/null input_small.ppm
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-decode ${COUNT_STATS} ./consumer-jpeg-decode -dct int -ppm -outfile /dev/null input_small.jpg
