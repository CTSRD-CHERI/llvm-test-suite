#!/bin/sh
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-smoothing ${COUNT_STATS} ./susan input_small.pgm /dev/null -s
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-edges ${COUNT_STATS} ./susan input_small.pgm /dev/null -e
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-corners ${COUNT_STATS} ./susan input_small.pgm /dev/null -c
