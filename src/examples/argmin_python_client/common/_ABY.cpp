/*
 *  _nneigh.cpp
 *    Python wrapper
 *
 *  by:  Roger Bermudez-Chacon
 */

#include <Python.h>
#include <numpy/arrayobject.h>
#include "argmin_python_client.h"
#include <stdint.h>

static char module_docstring[] =
    "Interface to compute argmin with ABY";
static char argmin_docstring[] = 
    "Calculate argmin";

static PyObject *ABY_argmin(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"argmin", ABY_argmin, METH_VARARGS, argmin_docstring},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC init_ABY(void) {
    PyObject *m = Py_InitModule3("_ABY", module_methods, module_docstring);
    if (m == NULL)
        return;

    import_array();
}

static PyObject *ABY_argmin(PyObject *self, PyObject *args) {
    PyObject *query_obj;
    // Parse input tuple args
    if (!PyArg_ParseTuple(args, "O", &query_obj)) return NULL;

    // Interpret query as numpy array
    PyObject *query_array = PyArray_FROM_OTF(query_obj, NPY_UINT32, NPY_IN_ARRAY);

    if (query_array == NULL) {
        Py_XDECREF(query_array);
        return NULL;
    }

    int D = (int) PyArray_DIM(query_array, 0);
    uint32_t* query = (uint32_t*) PyArray_DATA(query_array);

    uint32_t nneigh = argmin(query);
    uint32_t* db_size = get_db_size();

    if (db_size[1] != D) {
        PyErr_SetString(PyExc_RuntimeError,
                "Number of dimensions differs from database");
        return NULL;
    }

    Py_DECREF(query_array);

    free(query);
    free(db_size);

    // Output value
    PyObject *ret = Py_BuildValue("I", nneigh);
    return ret;
}
