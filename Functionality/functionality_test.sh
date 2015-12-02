#!/bin/sh

#  $2 = if 1 , ignore errors, make all tests 

error_handler(){
    
    if [ "$2" -eq 0 ]; then
        printf "$1\n" $i 
        rm ./Functionality/log
        exit 1
    else 
        printf "$1" $i 
        exit_code=1
    fi
}

files_counter=$(ls ./Functionality/ | grep "\.cpp$" | wc -l)
i=1
exit_code=0
memory_errors=0


while [ $i -le $files_counter ]; do 
    
    ./"$1" ./Functionality/$i.cpp 1> ./Functionality/log 2>/dev/null
    if [ $? -ne 0 ]; then        
      
        error_handler "\nNOT ZERO returned value: file $i " "$2"
    fi
    
    if [ `diff ./Functionality/log ./Functionality/Expected_results_$i | grep "<" | wc -l`  -ne 0 ]; then 

        error_handler "\nWrong output, file $i" "$2"       
    fi  


   valgrind --error-exitcode=1 ./"$1" ./Functionality/$i.cpp > /dev/null 2>&1
    if [ $? -ne 0 ]; then 
        memory_errors=$((memory_errors+1))
        error_handler "\nProblem with MEMORY LEAK file $i " "$2"
    fi

    printf "\nFILE $i done--"
    i=$((i+1))

done

if [ $memory_errors -eq 0 ]; then
    printf "\nNo memory leaks are possible\n" 
else
    printf "\nNumber of MEMORY LEAKS\n" $memory_errors
fi

rm ./Functionality/log

if [ $exit_code -ne 0 ]; then
    exit exit_code
fi