#bin/sh


i=0;

while read p; do 
    echo "\n\n----------------test----------------"
    i=$((i+1))  

    echo $p >> testforinterpret.txt

    ./$1 testforinterpret.txt
 
    if [ $? -neq 0 ]; then 
        echo "\nproblem na riadku" $i
        rm testforinterpret.txt
        exit 1
    fi
    
    done < $2;

    echo "\n\n------------------------------"
    echo "\nEverything OK lucky man!!!\n"
    rm testforinterpret.txt