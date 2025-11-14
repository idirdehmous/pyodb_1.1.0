#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define ODB_STRLEN 8  // 8 chars + '\0' (ODB1 string packé dans double)
#include <numpy/arrayobject.h>
#include <numpy/ndarraytypes.h> 

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
#include "pyspam.h"


// Fetch method  
static PyObject *odbFetch_method(PyObject *Py_UNUSED(self), PyObject *args, PyObject *kwargs)
{
    import_array();  // Numpy init 

    // Arguments keyword
    char *database  = NULL;
    char *sql_query = NULL;
    int   fcols     = 0;
    char *queryfile = NULL;
    char *poolmask  = NULL;
    int   fmt_float = 15  ;

    // Options Python (booléennes)
    PyObject *pbar  = Py_None;
    PyObject *pverb = Py_None;
    PyObject *phead = Py_None;
    PyObject *pnum  = Py_None;

    Py_INCREF(pbar  );
    Py_INCREF(pverb );
    Py_INCREF(phead );
    Py_INCREF(pnum  );
    

    Bool lpbar   = false;
    Bool verbose = false;
    Bool header  = false;
    Bool asnum   = false;

    // Keyword list 
    static char *kwlist[] = {  "database" , "sql_query", "nfunc","fmt_float",
                               "queryfile", "poolmask" , "pbar" , "verbose"  , 
			       "header","only_numeric", NULL
                             };

    // Lecture des arguments Python avec mots-clés
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssi|izzOOOO", kwlist,   // 3 requiered , 7 optional 
                                     &database,   
                                     &sql_query,  
                                     &fcols    ,
				     &fmt_float,
				     &queryfile,  
                                     &poolmask, 
                                     &pbar,
                                     &pverb,
                                     &phead, 
				     &pnum   )) {
        return NULL;
    }



    // Conversion to boolean C variable
    lpbar   = PyObj_ToBool ( pbar , lpbar      ) ; 
    verbose = PyObj_ToBool ( pverb , verbose   ) ; 
    header  = PyObj_ToBool ( phead , header    ) ; 
    asnum   = PyObj_ToBool ( pnum  , asnum     ) ;

    if (verbose && poolmask) {
        printf("Fetch data from pool # %s\n", poolmask);
    }

    char *varvalue = NULL;
    Bool print_mdi = true;
    //Bool raw = true;
    int maxlines = -1;
    void *h = NULL;
    int maxcols = 0;
    int rc = 0;

    size_t ip = 0;
    size_t prog_max = 0;
    int row_idx = 0;

    // Estimate the total number of rows 
    int total_rows = getMaxrows(database, sql_query);
    if (total_rows <= 0) {
        printf ("%s\n" , "Not able to fetch the total number of rows !")  ; 
        return PyErr_NoMemory();
       }
    // Progress bar  
    if ( lpbar  ) { prog_max = (size_t) total_rows ;  }

    // return   
    if (maxlines == 0) return PyLong_FromLong(rc);

    // verbosity 
    if (verbose) {
        if (sql_query)
            printf("Executing query from string: %s\n", sql_query);
        else if (queryfile)
            printf("Executing query from file   : %s\n", queryfile);
    }

    // Open  ODB (thread-safe ) 
    Py_BEGIN_ALLOW_THREADS
    h = odbdump_open(database, sql_query, queryfile, poolmask, varvalue, &maxcols);
    Py_END_ALLOW_THREADS


    if (!h || maxcols <= 0) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to open ODB or invalid number of columns");
        return NULL;
    }

    // Number of columns taking into account the number of functions in the query 
    int ncols = maxcols - fcols;


    // For the header  (insert the colname names  )
    PyObject *header_list = PyList_New(ncols );

    //main  buffers allocations 
    double *buffer   = (double *)malloc(sizeof(double) * (size_t)total_rows * (size_t)ncols);
    int   *is_string = (int*    )calloc((size_t)ncols, sizeof(int ) );
    char **strbufs   = (char**  )calloc((size_t)ncols, sizeof(char*));

    // Check NULL 
    if (!is_string || !strbufs) {
            if (is_string) free(is_string);
            if (strbufs)   free(strbufs)  ;
            free(buffer)    ;
            odbdump_close(h);
    return PyErr_NoMemory() ;
            }

    if (!buffer) {
	PyErr_SetString(PyExc_RuntimeError, "Failed to allocate memory buffer for numeric values");
        odbdump_close(h);
        return PyErr_NoMemory();
            }


    if (verbose)
        printf("Number of requested columns : %d\n", ncols);

    /*     ODB internal vars   */
    int new_dataset = 0;
    colinfo_t   *ci = NULL;
    int         nci = 0;
    double      *d  = NULL;
    ALLOCX(d, maxcols);

    Bool packed = false;
    int (*nextrow)(void *, void *, int, int *) =
        packed ? odbdump_nextrow_packed : odbdump_nextrow;
    int dlen = packed ? maxcols * (int)sizeof(*d) : maxcols;

    ll_t nrtot = 0;
    int nd = 0;

    double float_val ;

    /*    Main while loop  for reading  */
    while ((nd = nextrow(h, d, dlen, &new_dataset)) > 0) {
        if (lpbar) {   ++ip;    print_progress(ip, prog_max);   }

        // reallocate if needed   2x nrows_tot 
        if (row_idx >= total_rows) {
            total_rows *= 2;
            double *tmp = (double *)realloc(buffer, sizeof(double) * (size_t)total_rows * (size_t)ncols);
            if (!tmp) {
                free(buffer);
                odbdump_close(h);
                FREEX(d);
                return PyErr_NoMemory();
            }
            buffer = tmp;
        }
        
if (new_dataset) {
    // odb column structure 
    ci = odbdump_destroy_colinfo(ci, nci);
    ci = odbdump_create_colinfo(h, &nci);

    // if header 
    if (header) {
        Py_ssize_t hcol = -1;

        for (int i = 0; i < nci; i++) {
            colinfo_t *pci = &ci[i];
            const char *colname  = NULL;
            const char *scolname = NULL;   // for cols of dtype string 

            hcol++;

            // Cas 1 : no  numeric & their colnames 
            if (!asnum && pci->dtnum == DATATYPE_STRING) {
                if (pci->nickname && strlen(pci->nickname) > 0) {
                    scolname = pci->nickname;
                }
                else if (pci->name && strlen(pci->name) > 0) {
                    scolname = pci->name;
                }
                else {
                    scolname = "<unknown>";
                }

                PyObject *py_name = PyUnicode_FromString(scolname);
                PyList_SetItem(header_list, hcol, py_name);  /* transfert de référence */
            }

            // Cas 2 : numeric only  
            else if (asnum && pci->dtnum != DATATYPE_STRING ) {
                if (pci->nickname && strlen(pci->nickname) > 0) {
                    colname = pci->nickname;
                }
                else if (pci->name && strlen(pci->name) > 0) {
                    colname = pci->name;
                }
                else {
                    colname = "<unknown>";
                }

                PyObject *py_name = PyUnicode_FromString(colname);
                PyList_SetItem(header_list, hcol, py_name);
            }
	    else if (pci->dtnum == DATATYPE_STRING ) {
                if (pci->nickname && strlen(pci->nickname) > 0) {
		   colname = pci->nickname;
		}
		else if (pci->name && strlen(pci->name) > 0) {
		   colname = pci->name;
		}
		     else {
                    colname = "<unknown>";
                }
                PyObject *py_name = PyUnicode_FromString(colname );
                PyList_SetItem(header_list, hcol, py_name);
                   }

            else if (pci->dtnum  != DATATYPE_STRING ) {
                if (pci->nickname && strlen(pci->nickname) > 0) {
                   colname = pci->nickname;
                }
                else if (pci->name && strlen(pci->name) > 0) {
                   colname = pci->name;
                }
                     else {
                    colname = "<unknown>";
                }
                PyObject *py_name = PyUnicode_FromString(colname );
                PyList_SetItem(header_list, hcol, py_name);
                   }


        }  // for
    }  // if (header)

    new_dataset = 0;
}  

// Allocate  STRING buffers for each column
for (int i = 0; i < ncols; ++i) {
    colinfo_t *pci = &ci[i];
    if (pci->dtnum == DATATYPE_STRING) {
        is_string[i] = 1;
        if (!strbufs[i]) {
            //  total_rows * ODB_STRLEN
            strbufs[i] = (char*)malloc((size_t)total_rows * (size_t)ODB_STRLEN);
            if (!strbufs[i]) {
                // clean & abort
                for (int k = 0; k < ncols; ++k) {
                    if (strbufs[k]) free(strbufs[k]);
                }
                free(strbufs);
                free(is_string);
                free(buffer);
                odbdump_close(h);
                FREEX(d);
                return PyErr_NoMemory();
            }
        }
    }
}

// Reallocate  maxrows   is reached 
if (row_idx >= total_rows) {
    total_rows *= 2;
    double *tmp = (double*)realloc(buffer, sizeof(double) * (size_t)total_rows * (size_t)ncols);
    if (!tmp) {
        // clean
        for (int k = 0; k < ncols; ++k) {
            if (strbufs[k]) free(strbufs[k]);
        }
        free(strbufs);
        free(is_string);
        free(buffer);
        odbdump_close(h);
        FREEX(d);
        return PyErr_NoMemory();
    }
    buffer = tmp;

    // Reallocate strings 
    for (int k = 0; k < ncols; ++k) {
        if (is_string[k]) {
            char *stmp = (char*)realloc(strbufs[k], (size_t)total_rows * (size_t)ODB_STRLEN);
            if (!stmp) {
                // clean
                for (int kk = 0; kk < ncols; ++kk) {
                    if (strbufs[kk]) free(strbufs[kk]);
                }
                free(strbufs);
                free(is_string);
                free(buffer);
                odbdump_close(h);
                FREEX(d);
                return PyErr_NoMemory();
            }
            strbufs[k] = stmp;
        }
    }
}

        // Read columns  
        for (int i = 0; i < ncols; ++i) {
            colinfo_t *pci = &ci[i];

            // Set np  NaN if  <NULL>
            if (print_mdi && pci->dtnum != DATATYPE_STRING && fabs(d[i]) == mdi) {
                buffer[(size_t)row_idx * (size_t)ncols + (size_t)i] = NAN;
                continue;
            }

            // CHECK DATATYPE 
            switch (pci->dtnum) {
                case DATATYPE_INT4:
                    buffer[(size_t)row_idx * (size_t)ncols + (size_t)i] = (int)d[i];
                    break;

                case DATATYPE_YYYYMMDD:
                    buffer[(size_t)row_idx * (size_t)ncols + (size_t)i]   = (int)d[i];
                    break;			

                case DATATYPE_HHMMSS:
                    buffer[(size_t)row_idx * (size_t)ncols + (size_t)i] = (int)d[i];
                    break;
                
                case DATATYPE_STRING: {
                     union { char s[sizeof(double)]; double d; } u;
                     u.d = d[i];
                     char *dst = &strbufs[i][(size_t)row_idx * (size_t)ODB_STRLEN];
                     memcpy(dst, u.s, ODB_STRLEN)    ; // 8 bytes , no  '\0'
		     buffer[(size_t)row_idx * (size_t)ncols + (size_t)i] = NAN ;   // add NAN to have the same array shape ncols x nrows 
                   break;
                  }	
            default:
                 float_val   = (double)d[i];
                 double fval =  format_float ( float_val , fmt_float);
                 buffer[(size_t)row_idx * (size_t)ncols + (size_t)i] = fval   ;
                 break;
            }
        }

        // rows counter 
        ++row_idx;
        ++nrtot;

        if (maxlines > 0 && nrtot >= maxlines)
            break;
    }

    // close and clean 
    ci = odbdump_destroy_colinfo(ci, nci);
    odbdump_close(h);
    if (d) {
        FREEX(d);
        d = NULL;
    }


    /*  Build numpy array  */
    npy_intp dims[2] = { (npy_intp)row_idx, (npy_intp)ncols };
    PyObject *np_array = PyArray_SimpleNewFromData(2, dims, NPY_DOUBLE, (void *)buffer);
    if (!np_array) {
        free(buffer);
        return PyErr_NoMemory();
    }

    PyObject *capsule = PyCapsule_New(buffer, NULL, free_when_done);
    if (!capsule) {
        free(buffer);
        Py_DECREF(np_array);
        return PyErr_NoMemory();
    }
    if (PyArray_SetBaseObject((PyArrayObject *)np_array, capsule) < 0) {
        Py_DECREF(capsule);
        Py_DECREF(np_array);
        free(buffer);
        return PyErr_NoMemory();
    }
    if (verbose)
        printf("Returned %d rows × %d cols (NumPy double array)\n", row_idx, ncols);


 // Init dict for the columns containing strings
    PyObject *strings_dict = PyDict_New();
    if (!strings_dict) {
        Py_DECREF(np_array);
        for (int k = 0; k < ncols; ++k) if (strbufs[k]) free(strbufs[k]);
        free(strbufs);
        free(is_string);
        return PyErr_NoMemory();
           }
// Get strings values   (8 Bytes )
for (int i = 0; i < ncols; ++i) {
    if (!is_string[i]) continue;
    PyObject *sarr = makeString_data(strbufs[i], (npy_intp)row_idx, 8);

    if (!sarr) {
        Py_DECREF(strings_dict);
        Py_DECREF(np_array);
        for (int k = 0; k < ncols; ++k) if (strbufs[k]) free(strbufs[k]);
        free(strbufs);
        free(is_string);
        return PyErr_NoMemory();
    }

   //  Free with PyCapsule_New  
    PyObject *scaps = PyCapsule_New(strbufs[i], NULL, free_when_done);
    if (!scaps) {
        Py_DECREF(sarr);
        Py_DECREF(strings_dict);
        Py_DECREF(np_array);
        for (int k = 0; k < ncols; ++k) if (strbufs[k]) free(strbufs[k]);
        free(strbufs);
        free(is_string);
        return PyErr_NoMemory();
    }
 // Attach  to an array  pyobject 
     if (PyArray_SetBaseObject((PyArrayObject*)sarr, scaps) < 0) {
        Py_DECREF(scaps);
        Py_DECREF(sarr);
        Py_DECREF(strings_dict);
        Py_DECREF(np_array);
        for (int k = 0; k < ncols; ++k) if (strbufs[k]) free(strbufs[k]);
        free(strbufs);
        free(is_string);
        return PyErr_NoMemory();
    }
     // Create a key to get the value in Python side !
        PyObject *key = PyUnicode_FromString( "col_items"); 
        if (!key) {
           Py_DECREF(sarr);
           Py_DECREF(strings_dict);
           Py_DECREF(np_array);
        for (int k = 0; k < ncols; ++k) if (strbufs[k]) free(strbufs[k]);
        free(strbufs);
        free(is_string);
        return PyErr_NoMemory();
      }

    PyDict_SetItem(strings_dict, key, sarr);
    Py_DECREF(key);
    Py_DECREF(sarr);
    // strbufs has to be freed by PyCapsule 
    strbufs[i] = NULL;
}

// Free memory  
free(strbufs);
free(is_string);

// Retrun the global dict
PyObject *result = PyDict_New();
if (!result) {
    Py_DECREF(header_list );
    Py_DECREF(strings_dict);
    Py_DECREF(np_array    );
    return PyErr_NoMemory();
}


// header = false , asnum = false ;   --> 0  //   
// header = false , asnum = true  ;   --> 1  //  
// header = true  , asnum = false ;   --> 2  //   
// header = true  , asnum = true  ;   --> 3  // 

int cond  = (header << 1) | asnum ;
switch (cond) {
 case 0: //  no header only data (strings + numpy array )
        PyDict_SetItemString(result, "data_array", np_array) ;
        PyDict_SetItemString(result, "col_string" , strings_dict ) ;  
        Py_DECREF(np_array    );
        Py_DECREF(strings_dict); 
	break; 

 case 1: // no header , no string values ( pure numpy array )
        PyDict_SetItemString(result, "data_array", np_array);
        Py_DECREF(np_array);
        break;

 case 2:// header + columns dtype string + numpy array  
        PyDict_SetItemString(result, "header"    , header_list) ;
	PyDict_SetItemString(result, "data_array", np_array   ) ;
        PyDict_SetItemString(result, "col_string" , strings_dict ) ;
        Py_DECREF(header_list );
	Py_DECREF(np_array    );
	Py_DECREF(strings_dict);
	break ; 

 case 3:// header + numpy array 
	PyDict_SetItemString(result, "header"    , header_list) ;
        PyDict_SetItemString(result, "data_array", np_array   ) ;
        Py_DECREF(header_list );
        Py_DECREF(np_array    );
	break ; 
 default:// There is no other combination !
        Py_RETURN_NONE ; 
	break ; 

}
// Return array 
return result;
}


