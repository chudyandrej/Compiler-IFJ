#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Knuth-Morris-Pratt algorithm for string matching
 * @param  s      string array which is searched
 * @param  search string array which is used to search in s
 * @return        starting index of search string in s array (indexed from 0) 
 */
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

/**
 * Swap two array items given by indexes
 * @param  s pointer to char array
 * @param  i index the first item in array to exchange
 * @param  j index the second item in array to exchange
 */
void swap(char *s, int i, int j){
    char swap;
    swap = s[i];
    s[i] = s[j];
    s[j] = swap;
}

/**
 * Heapify function creates max heap from given array
 * @param  s      pointer to char array
 * @param  length length of array
 */
void heapify(char *s, int length){
    int index;      /*parents' indexes*/
    int j;          /*just counter*/
    int swap_controll=1;

    /*Find index of the last parent => round((size-1)/2) (indexing from 0) */
    /*Find left_child => parent_index*2+1 */
    /*Find right_child => parent_index*2+2 */
    while(swap_controll){
        swap_controll=0;
        for(index = 0; index <= (length-1)/2; index++){ /*loop each parent node*/
           
            for(j=1; j < 3; j++){
                if(s[index]<s[index*2+j]){
                    swap(s, index, index*2+j);  
                    swap_controll=1;              
                }
                if(index*2+2 == length+1) { break; }/*means only one child exist*/
            }
        }      
    }
}

/**
 * Shift the first item down till array is ordered
 * @param s      pointer to char array
 * @param length length of array
 */
void shift_down(char *s, int length){
    int index=0;

    while(index <= ((length-1)/2)){
        if(index*2+2 < length+1){   /*two children*/
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
}

/**
 * Heap sort algorithm
 * @param  s pointer to char array
 */
void sort(char *s){
    int length = strlen(s)-1; /*index of last item */ 
    
    heapify(s, length);     /*heap sort needs max heap*/
   
   for( ; length > 0; ){      
        swap(s, 0, length);   /*swap first and last*/
        length--;
                     
        if(length>=1){           /*shift down*/ 
            shift_down(s, length); 
        }
    }
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
    free(a);
    free(c);
    return 0;
}