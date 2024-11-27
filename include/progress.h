#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include "ansi_colors.h"

#define NUM_SECS 80
#define MAX_LENGTH   50 
#define PERCENTAGE(V, T) (MAX_LENGTH - (((T - V) * MAX_LENGTH) / T))








//void my_wait(){   usleep(100000);  }

//Compute the percentage
int     calculate_percentage(int count, int max) {
        int    pp=    ((float)count / max )* 100 ; 
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


/*int  main()
{
	size_t num_secs = 1, max_secs = NUM_SECS;
	printf("%s\n", "");
	print_progress(0, max_secs  );
	for (; num_secs <= max_secs; ++num_secs)
	{
        int pp=  calculate_percentage ( num_secs,    max_secs   ) ; 
		my_wait(1);
		print_progress(num_secs, max_secs );
	}
	return 0;
}*/

