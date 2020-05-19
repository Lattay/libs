#!/bin/bash
if [[ -z "$1" ]]
then
    echo "Usage: ./test_hash.sh ALGO_ID [DATA_LENGTH]"
    exit
fi

use_hash=$1
shift

if [[ -n "$1" ]]
then
    data_length=$1
    shift
else
    data_length=32
fi
gcc ./test_hash_random_data.c -o hash_random_gen -DUSE_HASH=$use_hash -DDATA_LENGTH=$data_length

./hash_random_gen > hash1.txt &
p1=$!
./hash_random_gen > hash2.txt &
p2=$!
./hash_random_gen > hash3.txt &
p3=$!
./hash_random_gen > hash4.txt

wait $p1
wait $p2
wait $p3

cat hash*.txt > big_hash.txt
rm hash*.txt

python ./find_collisions.py

rm big_hash.txt
