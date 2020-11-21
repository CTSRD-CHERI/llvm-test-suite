#!/bin/sh
${COUNT_STATS} ./bin/toast -fps -c data/large.au > /dev/null
STATCOUNTERS_PROGNAME=${STATCOUNTERS_PROGNAME}-untoast ${COUNT_STATS} ./bin/untoast -fps -c data/large.au.run.gsm > /dev/null
