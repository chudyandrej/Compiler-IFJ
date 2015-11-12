#!/bin/sh


valgrind_exit=0
exit_code=0

for file in `ls ./Errors | grep "\.cpp$"`; do
    echo "\n\n--------TEST_OTHER_ERRORS--------" >> log_invalid_values_test.txt
    echo "Input file name: $file" >> log_invalid_values_test.txt
    echo "Output by interpret: " >> log_invalid_values_test.txt
   
    ./"$1" $file >> log_invalid_values_test.txt
    echo "Return value of interpret: $?" >> log_invalid_values_test.txt
    
    valgrind ./"$1" $file > /dev/null
    if [ $? -eq 1 ]; then

        valgrind_exit=1
        exit_code=1
        if [ $2 -eq 0 ]; then
            echo "MEMORY LEAK, file: Errors/$file"
            exit 1
        fi
    fi
    echo "Valgrind return value: valgrind_exit" >> log_invalid_values_test.txt

done

exit exit_code
