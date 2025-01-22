#include  <stdio.h>
#include  "odbdump.h"

static int  getMaxrows ( char* database ,   char*  sql_query , int nfunc ) {  
  void *h         = NULL;
  char *poolmask  = NULL; 
  ll_t nrtot =   0;
  int maxlines    = -1;
  Bool print_mdi =true ; 
  int irow =0  ; 
  int maxcols     = 0;
  int nci         = 0   ; 
  colinfo_t *ci   = NULL;
    h     = odbdump_open(database, sql_query, NULL ,poolmask, NULL, &maxcols);
    if (h && maxcols > 0) {
    int new_dataset = 0   ;
    double *d       = NULL;      
    int nd                ; 
    int (*nextrow)(void *, void *, int, int *) = odbdump_nextrow;
    int dlen = (unsigned int) maxcols;
    ALLOCX(d, maxcols);
    while ( (nd = nextrow(h, d, dlen, &new_dataset)) > 0) {
        if (new_dataset) {
              ci = odbdump_destroy_colinfo(ci, nci);
              ci = odbdump_create_colinfo(h, &nci)  ; 
	      new_dataset=0  ; 
	}
	      for (  int i=0; i<nd; i++) {
                   colinfo_t *pci = &ci[i];
                  if (print_mdi && pci->dtnum != DATATYPE_STRING && ABS(d[i]) == mdi) {
                      irow++ ; 
                  }else {
                   switch (pci->dtnum) {
		case DATATYPE_STRING:
		      irow++ ;  break;
               case DATATYPE_YYYYMMDD:
                      irow++ ;  break ;
               case DATATYPE_HHMMSS:
                      irow++ ;  break ;
               case DATATYPE_INT4:
                      irow++ ;  break;
	       default:
                      irow++ ;  break;
		              }
            	     }
	     }		  
       if (maxlines > 0 && ++nrtot >= maxlines) break;	     
    }
    ci = odbdump_destroy_colinfo(ci, nci) ;
    odbdump_close(h);  
    FREEX(d);   
    }
   int ncols= (maxcols - nfunc ) ;     // requested cols - nfunctions in sql statement 
   int number_rows= (irow/ ncols)  ; 
return  number_rows    ; 
}    


