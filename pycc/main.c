#define PY_SSIZE_T_CLEAN
#include "pyspam.h"
#include "io_module.h"
#include "info_module.h"
#include "fetch_module.h"
#include "dca_module.h"
#include "gcdist.h"

static PyMethodDef module_methods[] = {
    {"odbConnect",  (PyCFunction)(void(*)(void))    odbConnect_method ,
     METH_VARARGS | METH_KEYWORDS,   "Create odb connection   "},
    {"odbClose"  ,  (PyCFunction)(void(*)(void))    odbClose_method  ,
     METH_VARARGS | METH_KEYWORDS,   "Close an opened ODB "},
    {"odbTables",  (PyCFunction)(void(*)(void))     odbTables_method ,
     METH_VARARGS | METH_KEYWORDS,   "Show all existing ODB tables   "},
    {"odbVarno",  (PyCFunction)(void(*)(void))      odbVarno_method ,
     METH_VARARGS | METH_KEYWORDS,   "Print all ODB varno, parameters and description "},
    {"odbFunctions",  (PyCFunction)(void(*)(void))  odbFunctions_method ,
     METH_VARARGS | METH_KEYWORDS,   "Print all the possible function that could be used in ODB sql statement "},
    {"odbFetch" ,  (PyCFunction)(void(*)(void))     odbFetch_method ,
     METH_VARARGS | METH_KEYWORDS,   "Fetch rows from  a given ECMA or CCMA  ODB database given sql query "},
    {"odbDca"  ,  (PyCFunction)(void(*)(void))      odbDca_method   ,
     METH_VARARGS | METH_KEYWORDS,   "Create DCA (Direct Column  Access )   files "},
    {"gcDist",  (PyCFunction)(void(*)(void))      gcMatrix_method  ,
     METH_VARARGS | METH_KEYWORDS,   "Compute the distances between 2 sets of N (lon,lat) points , returns a 2D  matrix (NxN) "},

};






// Module  definition
static struct PyModuleDef   odbmodule = {
    PyModuleDef_HEAD_INIT,
    "pyodb",
    "C/Python interface to access the ODB1 IFS/ARPEGE databases\nDeveloped after the ECMWF source code odb software (S. Saarinen 2000) !",
    -1,
    module_methods ,
     .m_slots =NULL
};


// Called first during python call
PyMODINIT_FUNC PyInit_pyodb (void) {
    PyObject*  m  ;
    PyObject* ModuleError ;
    m=PyModule_Create(&odbmodule);
    if ( m == NULL) {
        ModuleError = PyErr_NewException("Failed to create the module : pyodb", NULL, NULL);
        Py_XINCREF(ModuleError) ;
        return NULL;
}
    return m  ;
}

