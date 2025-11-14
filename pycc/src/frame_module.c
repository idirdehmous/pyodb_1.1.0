#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>





// Get odb rows as  pandas dataframe 
// Experimental module under dev  !!
static PyObject* odbFrame_method(PyObject* self, PyObject* args, PyObject* kwargs)
{

   PyObject *result = NULL;

    if (!PyArg_ParseTuple(args, "O", &result)) {
        PyErr_SetString(PyExc_TypeError, "odbFrame() expects one argument: the dict returned by odbFetch().");
        return NULL;
    }


    PyErr_SetString(PyExc_NotImplementedError, "Function not yet implemented !");
    return NULL ; 



    if (!PyDict_Check(result)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be a dictionary (result of odbFetch).");
        return NULL;
    }





PyObject *pandas = PyImport_ImportModule("pandas") ;
if (!pandas) {
    PyErr_SetString(PyExc_ImportError, "Cannot import pandas");
    return NULL;
}

// Get the DataFrame constructor
PyObject *pd_DataFrame = PyObject_GetAttrString(pandas, "DataFrame");
if (!pd_DataFrame) {
    Py_DECREF(pandas);
    PyErr_SetString(PyExc_AttributeError, "Cannot find pandas.DataFrame");
    return NULL;
}

   // Extract keys
    PyObject *np_array = PyDict_GetItemString(result, "data");
    PyObject *colnames = PyDict_GetItemString(result, "colnames");
    PyObject *strings  = PyDict_GetItemString(result, "strings");




   if (!np_array || !colnames) {
        Py_DECREF(pandas);
        Py_DECREF(pd_DataFrame);
        PyErr_SetString(PyExc_KeyError, "Missing 'data' or 'colnames' in dict.");
        return NULL;
    }

/*if (!PyArray_Check(np_array)) {
        Py_DECREF(pandas);
        Py_DECREF(pd_DataFrame);
        PyErr_SetString(PyExc_TypeError, "'data' must be a NumPy array.");
        return NULL;
    }*/


if (!PyArray_Check(np_array) || PyArray_NDIM((PyArrayObject*)np_array) != 2) {
    PyErr_SetString(PyExc_TypeError, "'data' must be a 2D NumPy array");
    return NULL;
}
npy_intp* shape = PyArray_SHAPE((PyArrayObject*)np_array);


Py_ssize_t ncols = PyList_Size(colnames);
    if (ncols <= 0) {
        Py_DECREF(pandas);
        Py_DECREF(pd_DataFrame);
        PyErr_SetString(PyExc_ValueError, "Empty column list in 'colnames'.");
        return NULL;
    }


  // Build a new Python dict { colname : column_data }
    PyObject *data_dict = PyDict_New();
    if (!data_dict) {
        Py_DECREF(pandas);
        Py_DECREF(pd_DataFrame);
        PyErr_NoMemory();
        return NULL;
    }


   // Add numeric columns
  /*  for (Py_ssize_t i = 0; i < ncols; ++i) {
        PyObject *col_name = PyList_GetItem(colnames, i);

        if (!col_name) continue;

        const char *cname = PyUnicode_AsUTF8(col_name);
	
        // Extract the i-th column from numpy array
         PyObject *col_data = PyObject_GetItem(np_array, PyLong_FromSsize_t(i));

        if (!col_data) {
            PyErr_Clear();
            continue;
        }
	PyDict_SetItemString(data_dict,  cname   , col_data);
        Py_DECREF(col_data);
    } NEED FLIP  */



// Add numeric columns
for (Py_ssize_t i = 0; i < ncols; ++i) {
    PyObject *col_name = PyList_GetItem(colnames, i);
    if (!col_name) continue;

    const char *cname = PyUnicode_AsUTF8(col_name);
    // Slice de la colonne i : np_array[:, i]
    PyObject *slice_all = PySlice_New(NULL, NULL, NULL);      // ":"
    PyObject *col_idx   = PyLong_FromSsize_t(i);              // i
    PyObject *idx_tuple = PyTuple_Pack(2, slice_all, col_idx);
    Py_DECREF(slice_all);
    Py_DECREF(col_idx);
    if (!idx_tuple) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate slice tuple");
        return NULL;
    }



    PyObject *col_data = PyObject_GetItem(np_array, idx_tuple);
    Py_DECREF(idx_tuple);
    if (!col_data) {
        PyErr_Clear();
        continue;
    }

    // Add  column data 
    PyDict_SetItemString(data_dict, cname, col_data);
    Py_DECREF(col_data);
}



// Add colname to the dict 
if (strings && PyDict_Check(strings)) {
    PyObject *k, *v;
    Py_ssize_t pos = 0;
    while (PyDict_Next(strings, &pos, &k, &v)) {
        const char *sname = PyUnicode_AsUTF8(k);
        if (!sname) continue;
        PyDict_SetItemString(data_dict, sname, v);
    }
}




  // Build DataFrame
    PyObject *args_df = PyTuple_Pack(1, data_dict);
    PyObject *kwargs_df = NULL;
    PyObject *df = PyObject_Call(pd_DataFrame, args_df, kwargs_df);


// free    pointers 
    Py_XDECREF(args_df);
    Py_XDECREF(data_dict);
    Py_XDECREF(pd_DataFrame);
    Py_XDECREF(pandas);

    if (!df) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create pandas.DataFrame.");
        return NULL;
    }

return NULL    ;

}
