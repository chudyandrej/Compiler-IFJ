#!/bin/sh

if [ $? -ne 1 ]; then 
    echo "Wrong number of parameters"
    echo "Use a file to compile as parameter"
    exit 1
fi


echo "--------Functionality test-----------"

bash ./Functionality/functionality_test.sh "$1"
if [ $? != 0 ]; then 
    echo "!!! ERROR !!!"
    exit 1
fi

echo "------Functionality test done--------"



echo "\n--------Error values test----------"
echo "Output of the test needs to be check by human!\n  - log_invalid_values_test.txt"

bash ./Errors/invalid_values_test.sh "$1"

echo "------Error values test done---------"



echo "\n--------Other error test-----------"

bash ./Errors/other_error_test.sh "$1"

echo "------Other error test done---------"

echo "\n---Test complete, check log_invalid_values_test.txt. and"
echo "if everything is OK, made more tests up!!!"

