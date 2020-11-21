#!/bin/sh
${COUNT_STATS} ./fft 4 4096 > /dev/null
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-inverse ${COUNT_STATS} ./fft 4 8192 -i > /dev/null
