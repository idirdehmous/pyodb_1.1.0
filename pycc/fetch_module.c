#define PY_SSIZE_T_CLEAN
#define CHUNK_SIZE 8192
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <Python.h>
#include <unistd.h>
#include "odbdump.h"
#include "rows.h" 
#include "progress.h"



/* ENDIANESS */
extern int ec_is_little_endian();
extern double util_walltime_();

// TODo 
// --> DEBUG ON/OFF 




static PyObject *odbFetch_method( PyObject* Py_UNUSED(self)  , PyObject*  args, PyObject* Py_UNUSED( kwargs )) {
    
    char *database   = NULL ;
    char *sql_query  = NULL ;
    char *queryfile  = NULL ;

    char *poolmask   = NULL ; 
    int   fcols      = 0    ; 
    char *fmt_float  = NULL ; 
    
    Py_Initialize()         ;

    PyObject *sh ; 
    PyObject *st;
    PyObject *dt ; 
    PyObject *ttm; 
    PyObject *int4; 
    PyObject *fl;
    PyObject* pbar     ; 
    PyObject* bool_arg1 ;
    PyObject* bool_arg2 ;
    Bool verbose        ;
    Bool header         ;
    Bool lpbar          ;

//static char *kwlist[] = {"database" , "sql_query" , "ncols" , "queryfile"};
//if (!PyArg_ParseTupleAndKeywords( kwargs, "ssi|z", kwlist, PyUnicode_FSConverter, &database , &sql_query ,&ncols,  &queryfile))
//{
//return NULL;
//}

if (!PyArg_ParseTuple (args , "ssi|zzzOOO" ,&database  , 
                                         &sql_query , 
					 &fcols     ,
					 &queryfile , 
					 &poolmask  , 
          				 &fmt_float ,
					 &pbar     , 
					 &bool_arg1  , 
         				 &bool_arg2  
					   ) )
    {
	    return NULL ;
    }


//  boolean variables conversion 
    lpbar    = PyObject_IsTrue(pbar  );
    verbose  = PyObject_IsTrue(bool_arg1);
    header   = PyObject_IsTrue(bool_arg2) ;


if ( verbose  )   {
  if (poolmask ) {
      printf ( "%s   %s\n" , "Fetch data from pool # ",  poolmask) ; 
  }
};


  char *varvalue  = NULL;
  Bool print_mdi  = true;      /* by default prints "NULL", not value of the NULL */
  Bool raw        = true;
  int maxlines    = -1;
  void *h         = NULL;
  int maxcols     = 0;
  int rc          = 0;

  char* delim=NULL ; 
  if (!delim) delim = STRDUP(" ");
  extern int optind;


  size_t ip   =0;
  size_t prog_max = 0  ;

 
  if ( lpbar  ) {
   int  total_rows =getMaxrows(database, sql_query , fcols   )  ; 
   prog_max = (size_t) total_rows ; 
  }

  if (maxlines == 0) return PyLong_FromLong(rc );
  if ( verbose ) {
   if ( sql_query  ) {
   printf ( "Executing query from string : %s\n" , sql_query ) ; 
   } else if ( queryfile) {
   printf ( "Executing query from file   : %s\n" , queryfile ) ;
   } ; 
  };


   // open the odb  ( isolate the thread )
   Py_BEGIN_ALLOW_THREADS
    h     = odbdump_open(database, sql_query, queryfile, poolmask, varvalue, &maxcols);
   Py_END_ALLOW_THREADS

   PyObject*  py_row    = PyList_New(0) ;
   Py_ssize_t irow      = -1  ;

  if (h && maxcols > 0) {

    if (verbose ) {
    printf( "Number of requested columns : %d \n" , maxcols - fcols )  ; 
    } ; 

    int new_dataset = 0   ;
    colinfo_t *ci   = NULL;
    int nci = 0           ; // NUMBER OF COLUMNS IN QUERY 
    double *d = NULL;       // DATA VALUES C VARIABLE 
    int nd          ; 
    ll_t nrows =   0;
    ll_t nrtot =   0;
    Bool packed = false;
    int (*nextrow)(void *, void *, int, int *) = 
      packed ? odbdump_nextrow_packed : odbdump_nextrow;
    int dlen = packed ? maxcols * sizeof(*d) :(unsigned int) maxcols;

    ALLOCX(d, maxcols);

    while ( (nd = nextrow(h, d, dlen, &new_dataset)) > 0) {	    

      if ( pbar  ) {
      ip=ip+1 ; 
      print_progress(ip  , prog_max  );
      }
      Py_ssize_t i ;
      PyObject*  py_col    = PyList_New(maxcols-fcols) ;

      char  nul[5] = "NULL";
      char *pnul  = nul ; 
      Py_ssize_t  icol= -1 ;
      
      if (new_dataset) {

	      ci = odbdump_destroy_colinfo(ci, nci);	
	      ci = odbdump_create_colinfo(h, &nci) ; 
	      
	      if (  header )  {
	         Py_ssize_t  hcol= -1    ;                           // For header only !
                 PyObject*  py_hcol    = PyList_New(maxcols-fcols) ; 
	
                 for (i=0; i<nci; i++) {
              
	             colinfo_t *pci = &ci[i];
              
	      sh=  PyUnicode_FromString(pci->nickname ? pci->nickname : pci->name  ) ;
              hcol++ ;
              
	      PyList_SetItem ( py_hcol   , hcol , sh  ) ; Py_INCREF( sh );
	
              
	           } //for 
	      PyList_Append( py_row ,    py_hcol  ) ;  // Append header first !
	      }            
	      new_dataset = 0; 
	      nrows = 0;
      }

      if(raw){
	for (  i=0; i<nd; i++) {
             colinfo_t *pci = &ci[i];
	     if (print_mdi && pci->dtnum != DATATYPE_STRING && ABS(d[i]) == mdi) {
	      icol++ ;
	      st=PyUnicode_FromString(  pnul ) ;
              PyList_SetItem ( py_col   , icol , st  ) ; Py_INCREF(st)  ;
	      } 

	 else {
	 switch (pci->dtnum) { 
         case DATATYPE_STRING:
                {

		icol++ ; 
                char cc[sizeof(double)+1];
                char *scc = cc ;
                union {  char s[sizeof(double)] ;double d;} u ;  u.d = d[i];
                for (long unsigned int js=0; js<sizeof(double); js++) { char c = u.s[js]; *scc++ = isprint(c) ? c : '8' ; } 
                *scc = '\0';
		st=PyUnicode_FromFormat (cc )  ;
                PyList_SetItem ( py_col , icol , st  ) ; Py_INCREF(st)  ; 
               }
	      break;
         case DATATYPE_YYYYMMDD:
               icol++  ;
               dt=PyLong_FromLong((int)d[i]) ;
	       PyList_SetItem ( py_col , icol ,dt  ) ; Py_INCREF(dt) ; 
	       break;
	  case DATATYPE_HHMMSS:
	       icol++  ;
	       ttm=PyLong_FromLong((int)d[i]) ;
               PyList_SetItem ( py_col , icol ,ttm ) ; Py_INCREF(ttm); 
               break; 
          case DATATYPE_INT4:
              icol++  ;
              int4=PyLong_FromLong((int)d[i]) ; 
              PyList_SetItem ( py_col , icol , int4  ) ;Py_INCREF(int4);
              break;
	  default:
              icol++ ;
              fl=PyFloat_FromDouble((double)d[i]  ) ;
              PyList_SetItem ( py_col , icol ,   fl   ) ;Py_INCREF(fl) ; 
              break;
	    } /* switch (pci->dtnum) */			  
	 }
	} /* for (i=0; i<nd; i++) */
       } /* if (!raw_binary)*/
      ++nrows;
      ++irow  ;

     PyList_Append(py_row , py_col   ) ;
     
//     if (  nrows > 300 ) {  
//	     printf ( "%s\n" , " NUMBER OF ROWS LIMITED TO  300 !!!!!!!!!! " ) ;
//	     break ; 
//      }
     if (maxlines > 0 && ++nrtot >= maxlines) break; /* while (...) */
    } /* while (...) */
    ci = odbdump_destroy_colinfo(ci, nci);
    rc = odbdump_close(h);
    FREEX(d);

 }    /* if (h && maxcols > 0) ... */
//     Py_ssize_t num_rows  = PyList_Size(py_row  )  ; 
     
//     PyObject*  nrows     = PyLong_FromSsize_t ( num_rows ) ;
    
   //  if ( nrows == PyLong_FromLong( 0 ) ){
   //      printf( "--The SQL request returned no data.\n") ;
	// printf( "--please check your SQL query ! \n" )   ;
  //   }
     //PyObject* cnt = reset_counter ( &num_rows  ) ; 

     return    py_row   ;
}

