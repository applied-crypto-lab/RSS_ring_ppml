#!/bin/bash

num_parties=$1
experiment=$2
id=$3
model_dir=$4

config_name=runtime-config-$num_parties


if (("$num_parties" == "3")); then
    declare -a sleep_times=(4 2 0)
elif (("$num_parties" == "5")); then
    declare -a sleep_times=(12 9 6 3 0)
else
    echo "invalid number of parties"
    exit
fi

index=$(($(($id)) - 1))

timestamp=$(date -d "today" +"%Y%m%d%H%M")
echo $timestamp

if [ "$experiment" = "minionn" ]; then
    declare -a batch_sizes=("1" "5" "10" "25" "50")
    for batch in "${batch_sizes[@]}"; do
        sleep ${sleep_times[$index]}
        ./rss_nn $id $config_name minionn 1000 $batch $model_dir | tee -a $1pc_minionn_$id_$timestamp.txt
    done

elif [ "$experiment" = "q_mobilenet" ]; then
    declare -a dims=("128" "160" "192" "224")
    declare -a batches=("1")
    for dim in "${dims[@]}"; do
        for alpha_ind in {0..3}; do
            for bat in "${batches[@]}"; do
                echo "sleep ${sleep_times[$index]}; ./rss_nn $id $config_name q_mobilenet $dim $alpha_ind $bat 20 2 $model_dir | tee -a $1pc_mobilenet_$id_$timestamp.txt"
                sleep ${sleep_times[$index]}
                ./rss_nn $id $config_name q_mobilenet $dim $alpha_ind $bat 20 2 $model_dir | tee -a $1pc_mobilenet_$id_$timestamp.txt
            done
        done
    done
else
    echo "invalid input exp"
    exit 1
fi
