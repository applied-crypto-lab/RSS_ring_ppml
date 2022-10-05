#!/bin/bash

config_name=runtime-config-3

declare -a sleep_times=(6 3 0)
index=$(($(($2)) - 1))

timestamp=$(date -d "today" +"%Y%m%d%H%M")
echo $timestamp

model_dir=$3

if [ "$1" = "minionn" ]; then
    declare -a batch_sizes=("1" "5" "10" "25" "50")
    for batch in "${batch_sizes[@]}"; do
        sleep ${sleep_times[$index]}
        ./rss_nn $2 $config_name minionn 1000 $batch $model_dir | tee -a 3pc_minionn_$timestamp.txt
    done

elif [ "$1" = "q_mobilenet" ]; then
    declare -a dims=("128" "160" "192" "224")
    declare -a batches=("1")
    for dim in "${dims[@]}"; do
        for alpha_ind in {0..3}; do
            for bat in "${batches[@]}"; do
                echo "sleep ${sleep_times[$index]}; ./rss_nn $2 $config_name q_mobilenet $dim $alpha_ind $bat 20 2 $model_dir | tee -a 3pc_mobilenet_$timestamp.txt"
                sleep ${sleep_times[$index]}
                ./rss_nn $2 $config_name q_mobilenet $dim $alpha_ind $bat 20 2 $model_dir | tee -a 3pc_mobilenet_$timestamp.txt
            done
        done
    done
else
    echo "invalid input exp"
    exit 1
fi
