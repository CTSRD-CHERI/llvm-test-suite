#! /bin/sh

#-
# Copyright (c) 2017 Alexandre Joannou
# Copyright (c) 2019-2020 Alex Richardson
# All rights reserved.
#
# This software was developed by SRI International and the University of
# Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
# ("CTSRD"), as part of the DARPA CRASH research programme.
#
# @BERI_LICENSE_HEADER_START@
#
# Licensed to BERI Open Systems C.I.C. (BERI) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  BERI licenses this
# file to you under the BERI Hardware-Software License, Version 1.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at:
#
#   http://www.beri-open-systems.org/legal/license-1-0.txt
#
# Unless required by applicable law or agreed to in writing, Work distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations under the License.
#
# @BERI_LICENSE_HEADER_END@
#

# Fail the whole script if one of the benchmarks dies
set -xe

####################
# global variables #
####################

OUTFILE=$(pwd)/mibench-$(date +%Y%m%d-%H_%M_%S)-statcounters.csv
DISCARD_RUN=2
SAMPLE_RUN=10
SIZE=large

###################
# options parsing #
###################

usage(){
    printf  "Usage: %s [-h] [-o outfile] [-d discardrun] [-r samplerun] [-s size] cpuname\n" $(basename $0)
    printf  "\t%-15s\t%s\n" "-h" "display this help message"
    printf  "\t%-15s\t%s\n" "-o outfile" "output statcounters in outfile (pah relative to $(pwd)) (default: $OUTFILE)"
    printf  "\t%-15s\t%s\n" "-d discardrun" "number of runs to discard (default: $DISCARD_RUN)"
    printf  "\t%-15s\t%s\n" "-r samplerun" "number of runs to sample (default: $SAMPLE_RUN)"
    printf  "\t%-15s\t%s\n" "-s size" "benchmark variant to run, one of {large,small} (default: $SIZE)"
    printf  "\t%-15s\t%s\n" "cpuname" "the cpu for which to run, one of {mips,cheri128,cheri256}"
}
while getopts "ho:d:r:s:" opt; do
    case $opt in
        h)
            usage
            exit 0
            ;;
        o)
            OUTFILE=$(pwd)/$OPTARG
            echo "statcounters output file set to $OUTFILE"
            ;;
        d)
            if [ $(echo $OPTARG | grep -E "^[0-9]+$") ]; then
                DISCARD_RUN=$OPTARG
                echo "discarding $DISCARD_RUN run(s)"
            else
                echo "-d argument $OPTARG must be a positive integer"
                exit 1
            fi
            ;;
        r)
            if [ $(echo $OPTARG | grep -E "^[0-9]+$") ]; then
                SAMPLE_RUN=$OPTARG
                echo "sampling $SAMPLE_RUN run(s)"
            else
                echo "-r argument $OPTARG must be a positive integer"
                exit 1
            fi
            ;;
        s)
            if [ $(echo $OPTARG | grep -E "small|large") ]; then
                SIZE=$OPTARG
                echo "using $SIZE variant of the benchmarks"
            else
                echo "-s argument $OPTARG must be one of {large,small}"
                exit 1
            fi
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            exit 1
            ;;
    esac
done
shift $(($OPTIND - 1))

#############
# cpu check #
#############

CPU=$1

COUNT_STATS=""
case $CPU in
    mips64|mips64-hybrid|mips64-purecap)
    COUNT_STATS=beri_count_stats
    ;;
    riscv64|riscv64-hybrid|riscv64-purecap)
    COUNT_STATS="minimal_count_stats --format csv"
    # FIXME: counters currently broken due to ASR bug
    COUNT_STATS="${COUNT_STATS} -e cycles -e time -e instret"
    ;;
    *)
    echo "!!!! please provide the cpu to run for which to run, one of mips64{,-hybrid,-purecap} or riscv64{,-hybrid,-purecap}"
    exit 1
    ;;
esac
export COUNT_STATS

# For the mips-asan case:
export LD_LIBRARY_PATH="$(pwd)/lib"

################
# benchmarking #
################

BENCH_FAMILY="automotive consumer network office security telecomm"
BENCH_AUTOMOTIVE="basicmath bitcount qsort susan"
BENCH_CONSUMER="jpeg" #BENCH_CONSUMER="jpeg lame mad tiff-v3.5.4 typeset"
BENCH_NETWORK="patricia dijkstra" #BENCH_NETWORK="dijkstra patricia"
BENCH_OFFICE="stringsearch" #BENCH_OFFICE="ghostscript ispell rsynth sphinx stringsearch"
BENCH_SECURITY="blowfish rijndael sha" # one left, pgp
BENCH_TELECOMM="CRC32 FFT adpcm" #BENCH_TELECOMM="CRC32 FFT adpcm gsm"

echo "---> Enabling stat counters csv mode"
export STATCOUNTERS_FORMAT=csv
export BENCHMARK_ROOT=$PWD

for family in $BENCH_FAMILY; do
    BENCH_LIST_var=BENCH_`echo $family | tr [:lower:] [:upper:]`
    #echo $BENCH_LIST_var
    eval BENCH_LIST=\$$BENCH_LIST_var
    #echo $BENCH_LIST
    for bench in $BENCH_LIST; do
        echo "---> Running $family - $bench benchmark"
        cd $BENCHMARK_ROOT/$family/$bench
        echo "---> Unsetting statcounters output file for discarded runs"
        unset STATCOUNTERS_OUTPUT
        unset STATCOUNTERS_ARCHNAME
        unset STATCOUNTERS_PROGNAME
        i=0
        while [ $i != $DISCARD_RUN ]
        do
            sh -x ./runme_$SIZE.sh 2> /dev/null || echo "Failed to run $BENCHMARK_ROOT/$family/$bench"
            i=$(($i+1))
        done
        echo "... discarded $DISCARD_RUN run(s)"
        export STATCOUNTERS_OUTPUT=$OUTFILE
        export STATCOUNTERS_ARCHNAME="$CPU"
        export STATCOUNTERS_PROGNAME="$family-$bench"
        echo "---> Reset statcounters output file to $STATCOUNTERS_OUTPUT for sampled runs"
        echo "---> Set statcounters archname to $STATCOUNTERS_ARCHNAME"
        echo "---> Set statcounters progname to $STATCOUNTERS_PROGNAME"
        i=0
        while [ $i != $SAMPLE_RUN ]
        do
            sh -x ./runme_$SIZE.sh || echo "Failed to run $BENCHMARK_ROOT/$family/$bench"
            echo "... $family - $bench $SIZE benchmark run $i done"
            i=$(($i+1))
        done
        echo "---> Done running $family - $bench benchmark"
    done
done

echo "---> Disabling stat counters"
unset STATCOUNTERS_FORMAT
unset STATCOUNTERS_OUTPUT
unset STATCOUNTERS_ARCHNAME
unset STATCOUNTERS_PROGNAME
echo "===== DONE RUNNING BENCHMARKS ====="


exit 0
