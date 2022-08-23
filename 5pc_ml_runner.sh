#!/bin/bash

config_name=runtime-config-lake-5

declare -a sleep_times=(12 9 6 3 0)
index=$(($(($2))-1))

timestamp=$(date -d "today" +"%Y%m%d%H%M")
echo $timestamp

if [ "$1" = "minionn" ];
then
    declare -a batch_sizes=("1" "5" "10" "25" "50")
    for batch in "${batch_sizes[@]}"
    do
        sleep ${sleep_times[$index]}; ./build/rss_nn $2 $config_name minionn 1000 $batch /home/anbaccar/rss_nn_v2/models | tee -a 5pc_minionn_$timestamp.txt
    done

elif [ "$1" = "q_mobilenet" ];
then
    # declare -a dims=("128" "160" "192" "224")
    declare -a dims=("192" "224")
    # declare -a alphas=("0.25" "0.5" "0.75" "1.0")
    declare -a batches=("1" "10")
    # declare -a dims=("128")
    # declare -a batches=("1")
    for dim in "${dims[@]}"
    do
        for alpha_ind in {0..3}
        do
            for bat in "${batches[@]}"
            do
                echo "sleep ${sleep_times[$index]}; ./build/rss_nn $2 $config_name q_mobilenet $dim $alpha_ind $bat 5 1 /home/anbaccar/rss_nn_v2/models | tee -a 5pc_mobilenet_$timestamp.txt"
                sleep ${sleep_times[$index]}; ./build/rss_nn $2 $config_name q_mobilenet $dim $alpha_ind $bat 5 1 /home/anbaccar/rss_nn_v2/models | tee -a 5pc_mobilenet_$timestamp.txt
            done
        done
    done
elif [ "$1" = "eda_rb_pregen" ]; then
    declare -a dims=("128" "160" "192" "224")
    declare -a batches=("1" "10")

    for dim in "${dims[@]}"; do
        for alpha_ind in {0..3}; do
            for bat in "${batches[@]}"; do
                echo "sleep ${sleep_times[$index]}; ./build/rss_nn $2 $config_name eda_rb_pregen $dim $alpha_ind $bat 3 1 /home/anbaccar/rss_nn_v2/eda_rb_optimizations.txt | tee -a 5pc_eda_rb_pregen_$timestamp.txt"
                sleep ${sleep_times[$index]}
                ./build/rss_nn $2 $config_name eda_rb_pregen $dim $alpha_ind $bat 3 1 /home/anbaccar/rss_nn_v2/eda_rb_optimizations.txt | tee -a 5pc_eda_rb_pregen_$timestamp.txt
            done
        done
    done
else
    echo "invalid input exp"
    exit 1
fi






# echo $index


