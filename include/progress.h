#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include "ansi_colors.h"

#define NUM_SECS 80
#define MAX_LENGTH   50 
#define PERCENTAGE(V, T) (MAX_LENGTH - (((T - V) * MAX_LENGTH) / T))





//Compute the percentage
int     calculate_percentage(int count, int max) {

	int pp= 0; 
	if (  max != 0 ) {
        pp=    ((float)count / max )* 100 ; 
	}

        return pp  ;  
	
}


void print_progress(size_t count, size_t max )
{
	int ii;
	const char prefix[] = "[";
	const char suffix[] = "]";
	const size_t prefix_length = sizeof(prefix) - 1;
	const size_t suffix_length = sizeof(suffix) - 1;
	char *buffer = calloc( MAX_LENGTH  + prefix_length + suffix_length + 1, 1);
	strcpy(buffer, prefix);
	for ( ii=0 ; ii < MAX_LENGTH ; ++ii)	{		
		buffer[prefix_length + ii] = ii <(int) PERCENTAGE(count, max) ? '#' : ' ';
	}



	if ( count  <max   ) {
        int  pp  =calculate_percentage( count ,  max)  ; 
        char spp[20]  ;  // int to str 
	char smax[20] ; 


        snprintf(spp , sizeof(spp), "%d"  , pp );
	snprintf(smax , sizeof(smax), "%ld", max);


	strcpy(&buffer[prefix_length + ii], suffix);
	 
	printf("\b\r %c[2K \r%s Number of rows:%d   %s%s%s"   ,27,  buffer, pp ,   BCYN "Progress  ", spp,"% " reset );
	
	fflush(stdout);
	} else {

        int  pp  =calculate_percentage( count ,  max)  ;
        char spp[20];  
	char smax[20] ; 
        snprintf(spp , sizeof(spp), "%d", pp );
        snprintf(smax , sizeof(smax), "%ld", max);

        printf("\b\r %c[2K \r%s Total rows:%s   %s%s%s\n"   ,27,  buffer, smax, BGRN "Complete  ", spp,"% "  );
	printf(  reset  ""    ) ; 
        fflush(stdout);
	}
	free(buffer);
}


