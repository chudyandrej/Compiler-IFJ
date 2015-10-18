#!/bin/sh




while read p; do 
    echo $p > file.txt
    ./scanner file.txt
    echo "----------------------------"         
    
done < $1;  

rm file.txt