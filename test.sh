#!/bin/sh

if [ $# -eq 0 ]; then 
    printf "\nWRONG number of parameters!!!!\n"
    printf "Parameters: \n  - \$1 = interpret\n  - \$2 [optional] -all = makes all tests"
    printf "even if errors occures\n\n" 
    exit 1
fi

if [ "$2" = "-all" ]; then
   controll=1
else 
   controll=0
fi

exit_code=0


echo "-----------FUNCTIONALITY_TEST-------------"

bash ./Functionality/functionality_test.sh "$1" "$controll"
if [ $? -ne 0 ]; then 
    
    if [ $controll -eq 0 ]; then
        echo "!!! ERROR !!!"
        exit 1
    else
        exit_code=1
    fi
fi

echo "--------FUNCTIONALITY_TEST_DONE-----------"




printf "\n----------ERROR_VALUES_TEST-------------\n"
printf "Output of the test needs to be check by human!\n  - log_invalid_values_test.txt\n"

bash ./Errors/invalid_values_test.sh "$1" "$controll"
if [ $? -ne 0 ]; then 
    if [ $controll -eq 0 ]; then
        echo "!!! ERROR !!!"
        rm ./valgrind_log
        exit 1
    else
        exit_code=1
    fi
fi

echo "---------ERROR_VALUES_TEST_DONE-----------"




printf "\n-----------OTHER_ERROR_TEST-------------\n"

bash ./Errors/other_error_test.sh "$1"
if [ $? -ne 0 ]; then 
    if [ $controll -eq 0 ]; then
        echo "!!! ERROR !!!"
        rm ./valgrind_log
        exit 1
    else
        exit_code=1
    fi
fi

echo "----------OTHER_ERROR_TEST_DONE-----------"

rm ./valgrind_log

if [ $exit_code -eq 1 ]; then
    echo "TESTS FAILD, check where!"
else
    printf "\n---Test complete, check log_invalid_values_test.txt and"
    echo " if everything is OK, made more tests up!!!"
fi

if [ $exit_code -ne 0 ]; then
    exit exit_code
fi