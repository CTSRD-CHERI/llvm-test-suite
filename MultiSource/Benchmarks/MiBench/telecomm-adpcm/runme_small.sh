#!/bin/sh
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-encode ${COUNT_STATS} ./telecomm-adpcm-encode < small.pcm > /dev/null
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-decode ${COUNT_STATS} ./telecomm-adpcm-decode < small.adpcm > /dev/null
