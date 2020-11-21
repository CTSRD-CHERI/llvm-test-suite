#!/bin/sh
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-encode ${COUNT_STATS} ./telecomm-adpcm-encode < large.pcm > /dev/null
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-decode ${COUNT_STATS} ./telecomm-adpcm-decode < large.adpcm > /dev/null
