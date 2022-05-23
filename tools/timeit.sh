#! /bin/bash
# A wrapper over perf to provide similar functionality to timeit.c

REPORT=/dev/stderr
INPUT=/dev/stdin
OUTPUT=/dev/stdout
ERRPUT=/dev/stderr
PERFSTAT=perfstats
APPEND_STATUS=0

while :; do
  case $1 in
  --summary)
    REPORT=$2
    PERFSTAT="$REPORT.perfstats"
    ;;
  --redirect-input)
    INPUT=$2
    ;;
  --redirect-output)
    OUTPUT=$2
    ERRPUT=$2
    ;;
  --redirect-stdout)
    OUTPUT=$2
    ;;
  --chdir)
    cd $2 || exit 1
    ;;
  --append-exitstatus)
    APPEND_STATUS=1
    shift 1
    continue
    ;;
  --limit-core)
    ulimit -c "$2"
    ;;
  --limit-cpu)
    ulimit -t "$2"
    ;;
  --limit-file-size)
    ulimit -f "$2"
    ;;
  --limit-rss-size)
    ulimit -m "$2"
    ;;
  --timeout)
    # Not implemented in timeit.sh
    ;;
  -*)
    echo "Error: unknown command line option $1 $2"
    exit 1
    ;;
  *)
    break
    ;;
  esac
  shift 2
done

if [ "$OUTPUT" = "$ERRPUT" ]; then
  # Use >& to ensure the streams are properly interleaved.
  perf stat -o $PERFSTAT $@ < $INPUT >& $OUTPUT
else
  perf stat -o $PERFSTAT $@ < $INPUT > $OUTPUT 2> $ERRPUT
fi

EXITCODE=$?
if [ "$APPEND_STATUS" = "1" ]; then
  echo "exit $EXITCODE" >> "$OUTPUT"
  # When --append-exitstatus is used, timeit will append the
  # exit status to the output file and return 0. Model this
  # behaviour here.
  EXITCODE=0
fi

echo exit $EXITCODE > "$REPORT"
awk -F' ' '{if ($2 ~ /^task-clock/ || $3 ~ /^task-clock/) {gsub(/,/,"",$1);print "user",$1/1000;} else if ($2 == "seconds" && $4 == "elapsed") {print "real",$1;} else if ($2 ~ /^instructions/) {gsub(/,/,"",$1);print "instructions",$1}}' $PERFSTAT >> $REPORT

exit $EXITCODE
