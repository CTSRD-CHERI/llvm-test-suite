#!/bin/sh
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-smoothing ${COUNT_STATS} ./susan input_large.pgm /dev/null -s
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-edges ${COUNT_STATS} ./susan input_large.pgm /dev/null -e
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-corners ${COUNT_STATS} ./susan input_large.pgm /dev/null -c
