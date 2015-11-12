#!/bin/sh

if [ $# -eq 0 ]; then 
    echo "\nWrong number of parameters"
    echo "Use a file to compile as parameter\n"
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
if [ $? != 0 ]; then 
    
    if [ $controll -eq 0 ]; then
        echo "!!! ERROR !!!"
        exit 1
    else
        exit_code=1
    fi
fi

echo "--------FUNCTIONALITY_TEST_DONE-----------"




echo "\n----------ERROR_VALUES_TEST-------------"
echo "Output of the test needs to be check by human!\n  - log_invalid_values_test.txt"

bash ./Errors/invalid_values_test.sh "$1" "$controll"
if [ $? != 0 ]; then 
    if [ $controll -eq 0 ]; then
        echo "!!! ERROR !!!"
        exit 1
    else
        exit_code=1
    fi
fi

echo "---------ERROR_VALUES_TEST_DONE-----------"




echo "\n-----------OTHER_ERROR_TEST-------------"

bash ./Errors/other_error_test.sh "$1"
if [ $? != 0 ]; then 
    if [ $controll -eq 0 ]; then
        echo "!!! ERROR !!!"
        exit 1
    else
        exit_code=1
    fi
fi

echo "----------OTHER_ERROR_TEST_DONE-----------"



if [ $exit_code -eq 1]; then
    echo "TEST FAILD, check where!"
else
    echo "\n---Test complete, check log_invalid_values_test.txt. and"
    echo "if everything is OK, made more tests up!!!"
fi

exit exit_code
