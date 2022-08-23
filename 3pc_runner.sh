#!/bin/bash

config_name=runtime-config-lake

timestamp=$(date -d "today" +"%Y%m%d%H%M")
echo $timestamp


source shared_vars_$1_$3.sh

# if [ "$1" = "30" ];
# then
#     source shared_vars_30.sh
# elif [ "$1" = "60" ];
# then
#     source shared_vars_60.sh
# else
#     echo "invalid ring size, must be 30 or 60"
#     exit 1
# fi

declare -a sleep_times=(4 2 0)
index=$(($(($2))-1))
# echo $index

for exp in "${exps[@]}"
do
    sleep ${sleep_times[$index]}; ./build/rss_nn $2 $config_name micro $exp | tee -a $1_3pc_micro_$3_$timestamp.txt
done

