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
 * @param  i index the first item in array 's' to exchange
 * @param  j index the second item in array 's' to exchange
 */
void swap(char *s, int i, int j){
    char swap;
    swap = s[i];
    s[i] = s[j];
    s[j] = swap;
}


/**
 * Reorder array (tree) by sorting heap tree
 * @param  s        pointer to char array
 * @param  start    index of nodeon which level is sift executed
 * @param  length   actual length of array 's'
 */
void sift_down(char *s, int start, int length) {
    int parent = start;

    while ( 2*parent + 1 < length) {
        int child = 2*parent + 1;
        if (child + 1 < length) { /* have both right and left childs */
            if (s[child] < s[child+1]) {
                child += 1; /* set bigger child to be swapped unit */
            }
        }
        if (s[parent] < s[child]) {
            swap(s, parent, child); /* swap if child is bigger then parent */
            parent = child;
        }
        else {
            break;
        }
    }
}


/**
 * Sort characters in array with heap-sort algoritm
 * @param  s        pointer to char array
 * @param  length   actual length of array 's'
 */
void sort(char *s) {
    int start, end;
    int length = strlen(s);

    /* heapify => make heap tree*/
    for (start = (length - 2)/2; start >= 0; start--) {
        sift_down(s, start, length);
    }

    for (end = length -1; end > 0; end--) {
        swap(s, end, 0);
        sift_down(s, 0, end);
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