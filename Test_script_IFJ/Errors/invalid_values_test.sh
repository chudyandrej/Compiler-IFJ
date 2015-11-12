#!/bin/sh


i=1;
exit_code=0
valgrind_exit=0

echo "" > log_invalid_values_test.txt   #overwrite file if exists

while read line; do

    echo "-----Test number $i-----" >> log_invalid_values_test.txt

    echo "int main(){" > ./Errors/input
    echo $line  >> ./Errors/input
    echo "}"  >> ./Errors/input

    echo "Input: $line"
    echo "Output by interpret:"

    ./"$1" ./Errors/input >> log_invalid_values_test.txt
    echo "Return value of interpret: $?" >> log_invalid_values_test.txt

    valgrind ./"$1" ./Errors/input > /dev/null
    if [ $? -eq 1 ]; then
        valgrind_exit=1
        exit_code=1
    fi
    echo "Valgrind return value: $valgrind_exit\n" >> log_invalid_values_test.txt

    i=$((i+1))

done < ./Errors/invalid_value_inputs; 

rm ./Errors/input
exit exit_code