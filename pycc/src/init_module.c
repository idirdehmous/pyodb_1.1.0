#define PY_SSIZE_T_CLEAN
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <Python.h>

//  THIS IS A DRAFT OF INIT MODULE !!!
//  //Init some specific python environment veriables
//  //Init some ODB envs 
// C wrappers
static PyObject* pyodbInit_method( PyObject *Py_UNUSED(self)){
// INIALISATION OF PYTHON ENVIRONNEMT AND SOME SPECIFIC ODB VARIBALES !
// pyodb can run whitout , just for more rebustness 
// THIS MODDULE IS UNDER DEV !
    printf ("%s\n" ,"--------------------------------------------------" )  ;
    printf ("%s\n", "THE MODULE  pyodbInit  IS UNDER DEVELOPMENT !" )  ;
    printf ("%s\n" ,"--------------------------------------------------" )   ;
//Py_GetBuildInfo();   
//Py_GetCompiler()   
//Py_GetCopyright()
//Py_GetPlatform()
//Py_GetVersion()
return NULL ;
}

