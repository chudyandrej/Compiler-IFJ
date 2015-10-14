#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main (int argc, char *argv[]){
    
   int c;
	if (argc < 2){
		fprintf(stdout,"Usage: %s number\n",argv[0]);
		return 1;
	}
	
	double inputValue = atof(argv[1]);
	
	fprintf(stdout,"Theof %g is \n",inputValue);
  
  	return 0;
} 