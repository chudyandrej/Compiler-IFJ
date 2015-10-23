#!/bin/sh


i=1

while [ $i -le 8 ]; do 
    
    ./"$1" ./Functionality/$i.cpp > ./Functionality/log
    if [ $? != 0 ]; then 
        echo "\nProblem with file " $i 
        rm ./Functionality/log
        exit 1
    fi
    
    diff ./Functionality/log ./Functionality/Expected_results_$i    
    if [ $? != 0 ]; then 
        echo "\nWrong output, file" $i 
        rm ./Functionality/log
        exit 1
    fi  

    echo "File $i was successfully interpreted "  

    valgrind ./"$1" ./Functionality/$i.cpp > /dev/null
    if [ $? != 0 ]; then 
        echo "\nProblem with memory leak, file " $i 
        rm ./Functionality/log
        exit 1
    fi

    echo "No memory leaks are possible: file $i" 

    i=$((i+1))
    
done
rm ./Functionality/log