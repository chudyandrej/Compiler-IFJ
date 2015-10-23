#!/bin/sh


for file in `ls ./Errors | grep "\.cpp$"`; do
    echo "\n\n--------Test other errors--------" >> log_invalid_values_test.txt
    echo "Input file name: $file" >> log_invalid_values_test.txt
    echo "Output by interpret: " >> log_invalid_values_test.txt
   
    ./"$1" $file >> log_invalid_values_test.txt
    echo "Return value of interpret: $?" >> log_invalid_values_test.txt
    
    valgrind ./"$1" $file > /dev/null
    echo "Valgrind return value: $?" >> log_invalid_values_test.txt

done
