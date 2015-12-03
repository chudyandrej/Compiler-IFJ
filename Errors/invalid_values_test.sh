#!/bin/sh


i=1;
exit_code=0

echo "" > log_invalid_values_test.txt   #overwrite file if exists

while read line; do

    printf "\n-----Test number $i-----\n" >> log_invalid_values_test.txt

    echo "int main(){" > ./Errors/input
    echo $line  >> ./Errors/input
    echo "return 0; }"  >> ./Errors/input

    echo "Input: $line" >> log_invalid_values_test.txt
    printf "Output by interpret:\n" >> log_invalid_values_test.txt

    ./"$1" ./Errors/input 1>> log_invalid_values_test.txt 
    echo "Return value of interpret: $?" >> log_invalid_values_test.txt

    valgrind ./"$1" ./Errors/input 2> valgrind_log 1>&2
    cat valgrind_log | grep "in use at exit" >> log_invalid_values_test.txt
    cat valgrind_log | grep "total heap usage" >> log_invalid_values_test.txt
    cat valgrind_log | grep "ERROR SUMMARY" >> log_invalid_values_test.txt
    

    echo "Test number $i done"
    i=$((i+1))

done < ./Errors/invalid_value_inputs; 


while read line; do

    printf "\n-----Test number $i-----\n" >> log_invalid_values_test.txt

    echo $line  > ./Errors/input
    echo "int main(){" >> ./Errors/input
    echo "return 0; }"  >> ./Errors/input

    echo "Input: $line" >> log_invalid_values_test.txt
    printf "Output by interpret:\n" >> log_invalid_values_test.txt

    ./"$1" ./Errors/input 1>> log_invalid_values_test.txt 
    echo "Return value of interpret: $?" >> log_invalid_values_test.txt


   valgrind ./"$1" ./Errors/input 2> valgrind_log 1>&2
    cat valgrind_log | grep "in use at exit" >> log_invalid_values_test.txt
    cat valgrind_log | grep "total heap usage" >> log_invalid_values_test.txt
    cat valgrind_log | grep "ERROR SUMMARY" >> log_invalid_values_test.txt

    i=$((i+1))

done < ./Errors/other_errors;


rm ./Errors/input

if [ $exit_code -ne 0 ]; then
    exit exit_code
fi