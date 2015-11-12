#!/bin/sh


error_handler(){
    
    if [ "$2" -eq 0 ]; then
        echo "$1" $i 
        rm ./Functionality/log
        exit 1
    else 
        echo "$1" $i 
        exit_code=1
    fi
}

files_counter=$(ls ./Functionality/ | grep "\.cpp$" | wc -l)
i=1
exit_code=0
memory_errors=0


while [ $i -le $files_counter ]; do 
    
    ./"$1" ./Functionality/$i.cpp > ./Functionality/log
    if [ $? != 0 ]; then        
      
        error_handler "\nCannot interpret file " "$2"
    fi
    
    diff ./Functionality/log ./Functionality/Expected_results_$i    
    if [ $? != 0 ]; then 

        error_handler "\nWrong output, file" "$2"       
    fi  


    valgrind ./"$1" ./Functionality/$i.cpp > /dev/null
    if [ $? != 0 ]; then 
        memory_errors=$((memory_errors+1))
        error_handler "\nProblem with MEMORY LEAK file " "$2"
    fi


    i=$((i+1))
    
done

if [ $memory_errors -eq 0 ]; then
    echo "No memory leaks are possible: file $i" 
else
    echo "Number of MEMORY LEAKS" $memory_errors
fi

rm ./Functionality/log
exit exit_code