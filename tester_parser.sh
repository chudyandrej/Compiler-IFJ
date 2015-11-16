#bin/sh


i=0;

while read p; do 
    echo "\n\n----------------test----------------"
    i=$((i+1))  

    echo "test line input no. $i: $p      #####"
    echo "int main(){" > testforinterpret.txt
    echo $p >> testforinterpret.txt
    echo "}" >> testforinterpret.txt
    ./$1 testforinterpret.txt
 
    if [ $? -ne $3 ]; then 
        printf "\nproblem on line $i\n"
        rm testforinterpret.txt
        exit 1
    fi
    done < $2;

    printf "\n\n------------------------------"
    printf "\nEverything OK lucky man!!!\n"
    rm testforinterpret.txt