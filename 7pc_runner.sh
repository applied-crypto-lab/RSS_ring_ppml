#!/bin/bash

config_name=runtime-config-lake-7

timestamp=$(date -d "today" +"%Y%m%d%H%M")
echo $timestamp 

source shared_vars_$1_7pc_$3.sh

declare -a sleep_times=(12 10 8 6 4 2 0)
index=$(($(($2))-1))
# echo $index

for exp in "${exps[@]}"
do
    sleep ${sleep_times[$index]}; ./build/rss_nn $2 $config_name micro $exp | tee -a $1_7pc_micro_$3_$timestamp.txt
done

