#!/bin/sh


exit_code=0

 printf "\n\n--------TEST_OTHER_ERRORS--------\n" >> log_invalid_values_test.txt

for file in `ls ./Errors | grep "\.cpp$"`; do
   
    printf "\nInput file name: $file\n" >> log_invalid_values_test.txt
    printf "Output by interpret:\n" >> log_invalid_values_test.txt
   
    ./"$1" ./Errors/$file 1>> log_invalid_values_test.txt 2>/dev/null
    echo "Return value of interpret: $?" >> log_invalid_values_test.txt
    
    valgrind ./"$1" ./Errors/$file 2> valgrind_log 1>&2
    cat valgrind_log | grep "in use at exit" >> log_invalid_values_test.txt
    cat valgrind_log | grep "total heap usage" >> log_invalid_values_test.txt
    cat valgrind_log | grep "ERROR SUMMARY" >> log_invalid_values_test.txt

    echo "Test $file done--"
done

if [ $exit_code -ne 0 ]; then
    exit exit_code
fi