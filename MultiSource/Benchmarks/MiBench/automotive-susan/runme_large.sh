#!/bin/sh
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-smoothing ${COUNT_STATS} ./automotive-susan input_large.pgm /dev/null -s
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-edges ${COUNT_STATS} ./automotive-susan input_large.pgm /dev/null -e
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-corners ${COUNT_STATS} ./automotive-susan input_large.pgm /dev/null -c
