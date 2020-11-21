#!/bin/sh
${COUNT_STATS} ./bin/toast -fps -c data/small.au > /dev/null
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-untoast ${COUNT_STATS} ./bin/untoast -fps -c data/small.au.run.gsm > /dev/null
