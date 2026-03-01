#define PY_SSIZE_T_CLEAN
#include "pyspam.h"
#include "io_module.h"
#include "info_module.h"
#include "dbarray_module.h"
#include "dca_module.h"
#include "dict_module.h"
#include "geo_module.h"



// Define the functions in the modules 
static PyMethodDef module_methods[] = {
    {"odbConnect",  (PyCFunction)(void(*)(void))    odbConnect_method ,
     METH_VARARGS | METH_KEYWORDS,   "Create odb connection object and init its environment"},

    {"odbClose"  ,  (PyCFunction)(void(*)(void))    odbClose_method  ,
     METH_VARARGS | METH_KEYWORDS,   "Close an opened ODB "},

    {"odbTables",  (PyCFunction)(void(*)(void))     odbTables_method ,
     METH_VARARGS | METH_KEYWORDS,   "Show all existing ODB tables   "},

    {"odbVarno",  (PyCFunction)(void(*)(void))      odbVarno_method ,
     METH_VARARGS | METH_KEYWORDS,   "Print all ODB varno, parameters and their description "},

    {"odbFunctions",  (PyCFunction)(void(*)(void))  odbFunctions_method ,
     METH_VARARGS | METH_KEYWORDS,   "Print all the possible functions that could be used in ODB sql statement"},

    {"odbArray" ,  (PyCFunction)(void(*)(void))     odbArray_method ,
     METH_VARARGS | METH_KEYWORDS,   "Fetch rows from a given ECMA or CCMA ODB database given sql query. This function returns a numpy. Suitable to speed up the selection if there only numeric data types in the query"},

    {"odbDict" ,  (PyCFunction)(void(*)(void))   odbDict_method ,
      METH_VARARGS | METH_KEYWORDS,   "Fetch rows as a python  dict"},

    {"odbDca"  ,  (PyCFunction)(void(*)(void))      odbDca_method   ,
     METH_VARARGS | METH_KEYWORDS,   "Create DCA  (Direct Column  Access )   files "},

    {"odbGcdist"  ,  (PyCFunction)(void(*)(void))      odbGcdist_method   ,
     METH_VARARGS | METH_KEYWORDS,   "Compute great circle distance between numpy latlon pairs "},

    {"odbGeopoints"  ,  (PyCFunction)(void(*)(void))   odbGeopoints_method  ,
     METH_VARARGS | METH_KEYWORDS,   "Fetch the lat,lon and obsvalue ONLY "},
};


// Module itself 
static struct PyModuleDef   odbmodule = {
    PyModuleDef_HEAD_INIT,
    "core",
    "C/Python interface to access the ODB1 IFS/ARPEGE databases\nThe original source code has been developed by Copyright (c) 1997-98, 2000 ECMWF. All Rights Reserved !",
    -1,
    module_methods ,
     .m_slots =NULL
};



// Called first during python call
PyMODINIT_FUNC PyInit_core (void) {

    import_array();   //   Init  Numpy C API  ... Otherwise  segfaults are everywhere  :-/

    PyObject*  m  ;
    PyObject* ModuleError ;
    m=PyModule_Create(&odbmodule);
    if ( m == NULL) {
        ModuleError = PyErr_NewException("Failed to create the module : core", NULL, NULL);
        Py_XINCREF(ModuleError) ;
        return NULL;
}

PyOdbEmptyResultError = PyErr_NewException("core.EmptyResultError", NULL, NULL);
Py_INCREF(PyOdbEmptyResultError);
PyModule_AddObject(m , "EmptyResultError", PyOdbEmptyResultError);

    return m  ;
}

