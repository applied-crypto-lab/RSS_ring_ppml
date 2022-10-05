config_name=runtime-config-$1

timestamp=$(date -d "today" +"%Y%m%d%H%M")
echo $timestamp

declare -a exps=(
    "mult $3 1 1 10"
    "mult $3 10 1 10"
    "mult $3 100 1 10"
    "mult $3 1000 1 10"
    "mult $3 10000 1 10"
    "mult $3 100000 1 10"
    "mult $3 1000000 1 10"
    "randbit $3 1 1 10"
    "randbit $3 10 1 10"
    "randbit $3 100 1 10"
    "randbit $3 1000 1 10"
    "randbit $3 10000 1 10"
    "randbit $3 100000 1 10"
    "randbit $3 1000000 1 10"
    "edabit $3 1 1 10"
    "edabit $3 10 1 10"
    "edabit $3 100 1 10"
    "edabit $3 1000 1 10"
    "edabit $3 10000 1 10"
    "edabit $3 100000 1 10"
    "edabit $3 1000000 1 10"
    "msb_rb $3 1 1 10"
    "msb_rb $3 10 1 10"
    "msb_rb $3 100 1 10"
    "msb_rb $3 1000 1 10"
    "msb_rb $3 10000 1 10"
    "msb_rb $3 100000 1 10"
    "msb_rb $3 1000000 1 10"
    "msb_eda $3 1 1 10"
    "msb_eda $3 10 1 10"
    "msb_eda $3 100 1 10"
    "msb_eda $3 1000 1 10"
    "msb_eda $3 10000 1 10"
    "msb_eda $3 100000 1 10"
    "msb_eda $3 1000000 1 10"
    "mat_mult $3 100 1 10"
    "mat_mult $3 10000 1 10"
    "mat_mult $3 250000 1 10"
    "mat_mult $3 1000000 1 10"
)
if (("$1" == "3")); then
    declare -a sleep_times=(4 2 0)
elif (("$1" == "5")); then
    declare -a sleep_times=(4 3 2 1 0)
elif (("$1" == "7")); then
    declare -a sleep_times=(12 10 8 6 4 2 0)
else
    echo "invalid number of parties"
    exit
fi
index=$(($(($2))-1))
# echo $index
# echo ${sleep_times[$index]}

for exp in "${exps[@]}"
do
    sleep ${sleep_times[$index]}; ./rss_nn $2 $config_name micro $exp | tee -a $1pc_micro_$2_$timestamp.txt
done
