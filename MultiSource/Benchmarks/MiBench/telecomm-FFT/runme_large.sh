#!/bin/sh
${COUNT_STATS} ./telecomm-fft 8 32768 > /dev/null
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-inverse ${COUNT_STATS} ./telecomm-fft 8 32768 -i > /dev/null
