#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*Knuth-Morris-Pratt algorithm for string matching*/
int find(char *s, char *search){       
    int pattern_length = strlen(search);    
    
    if (pattern_length == 0) { return 0; }
    
    int table[pattern_length];
    table[0] = 0;       /*must be zero*/
    int i=0; 
    int j=0; 
    
    /*fill table with values*/
    for(j = 1; j < pattern_length; j++){
        if(search[j] == search[i]){
            i++;
            table[j] = i;
        }
        else{
            while(i > 0 && search[i] != search[j]){
                i--;
            }
            if(i == 0 &&  search[i] == search[j]){
                table[j] = 1;
                i = 1;
            }
            else{
                table[j]=i;
            }     
        }
    }
    
    
    int source_length=strlen(s);
    j=0;
    
    for(i=0; i < source_length; i++){
        if(search[j] ==  s[i]){
            j++;
            if(j==pattern_length){      /*found search in s*/
                return i-j+1;
            }
        }
        else{
            if(j==0) continue;      /*go again and compare next char of string with pattern*/
            j=table[j-1];           /*save number according the table to j and */
            i--;                    /*compare again the same char of string with new char of pattern*/
        }        
    }    
    
    return -1;    
}

/*Swap two array item*/
int swap(char *s, int i, int j){
    char swap;
    swap = s[i];
    s[i] = s[j];
    s[j] = swap;

    return 0;
}

/*Heapify function = creates max heap from given array*/
int heapify(char *s, int length){
    int index;      /*parents' indexes*/
    int j;          /*just counter*/
    int swap_controll=1;

    /*Find index of the last parent => round((size-1)/2) (indexing from 0) */
    /*Find left_child => parent_index*2+1 */
    /*Find right_child => parent_index*2+2 */
    while(swap_controll){
        swap_controll=0;
        for(index = 0; index <= (length-1)/2; index++){
           
            for(j=1; j < 3; j++){
                if(s[index]<s[index*2+j]){
                    swap(s, index, index*2+j);  
                    swap_controll=1;              
                }
                if(index*2+2 == length+1) { break; }/*menas only one child exist*/
            }
        }      
    }
 
    return 0;
}

/*Shift the first item down till array is ordered*/
int shift_down(char *s, int length){
    int index=0;

    while(index <= ((length-1)/2)){
        if(index*2+2 < length+1){   /*two childs*/
            if(s[index*2+1] <= s[index*2+2] && s[index] < s[index*2+2]){
                swap(s, index, index*2+2);
                index=index*2+2;
            }
            else if(s[index*2+1] >= s[index*2+2] && s[index] < s[index*2+1]){
                swap(s, index, index*2+1);
                index=index*2+1;
            }
            else{
                break;
            }
        }
        else{       /*one child*/
            if(s[index] < s[index*2+1]){
                swap(s, index, index*2+1);
                index=index*2+1;
            } 
            else{
                break; 
            }
        }
    }
    return 0;
}

/*Heap sort algorithm*/
int sort(char *s){
    int length = strlen(s)-1; /*index of last item */ 
    
    heapify(s, length);     /*heap sort needs max heap*/
   
   for(length; length > 0; ){
        /*swap first and last*/
        swap(s, 0, length);
        length--;
        /*shift down*/       
       if(length>=1) { shift_down(s, length); }
    }
    return 0;
}

int main(int argc, char *argv[]){
    char *a=malloc(strlen(argv[1])*sizeof(char));
    char *c=malloc(strlen(argv[2])*sizeof(char));

    strcpy(a, argv[1]);
    strcpy(c, argv[2]);
    /*strcpy(a, "abrakasabraabrakadabra");
    strcpy(b, "abrakadabra");
    printf("%d\n", find(a,b));
    */
    printf("%d\n", find(a, c));
    sort(a);  
    printf("%s\n", a);
    return 0;
}