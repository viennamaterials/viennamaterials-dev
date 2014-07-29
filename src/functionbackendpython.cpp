/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "../viennamaterials/functionbackendpython.hpp"
#include <iostream> //XXX: testing

namespace viennamaterials
{

void function_backend_python::init(std::string code, std::string function_name)
{
  /// Initialize the Python Interpreter
  Py_Initialize();

  PyObject *value_ptr;
  global_ptr_ = PyDict_New();

  /// Create a new module object
  module_ptr_ = PyModule_New("mymod");
  PyModule_AddStringConstant(module_ptr_, "__file__", "");

  /// Get the dictionary object from my module so it can be passed to PyRun_String
  local_ptr_ = PyModule_GetDict(module_ptr_);

  /// Define the function in the newly created module
  value_ptr = PyRun_String(code.c_str(), Py_file_input, global_ptr_, local_ptr_);
  Py_DECREF(value_ptr);

  /// Get a pointer to the function
  function_ptr_ = PyObject_GetAttrString(module_ptr_, function_name.c_str());
}

function_backend_python::~function_backend_python()
{
  Py_XDECREF(function_ptr_);
  Py_DECREF(global_ptr_);
  Py_DECREF(local_ptr_);
  Py_DECREF(module_ptr_);

  /// Finish the Python Interpreter
  Py_Finalize();
}

double function_backend_python::eval() //XXX testing only
{
  /* TODO
   * get number of args
   * get info of args (quant, type, default)
   * get type of result
   *
   * wrap arguments into PyObjects
   *
   * wrap result into return object
   */

  if (PyCallable_Check(function_ptr_))
  {
    /// Build a tuple to hold arguments
    PyObject *arg_ptr;
    arg_ptr = PyTuple_New((Py_ssize_t) 2); //TODO: testing
    PyTuple_SetItem(arg_ptr, 0, PyFloat_FromDouble(3.0)); //TODO: testing
    PyTuple_SetItem(arg_ptr, 1, PyFloat_FromDouble(0.5)); //TODO: testing

    PyObject *value_ptr;
    value_ptr = PyObject_CallObject(function_ptr_, arg_ptr);
    std::cout << "return: " << PyFloat_AsDouble(value_ptr) << std::endl; //TODO: testing

    Py_DECREF(value_ptr);
    Py_DECREF(arg_ptr);
  } else
  {
    PyErr_Print();
  }

  return 1.0; //FIXME
}

xml_bool function_backend_python::eval(tag_scalar_bool tag)
{
  eval(); //FIXME: testing

  return true; //FIXME
}

xml_int function_backend_python::eval(tag_scalar_int tag)
{
  eval(); //FIXME: testing

  return 0; //FIXME
}

xml_float function_backend_python::eval(tag_scalar_float tag)
{
  eval(); //FIXME: testing

  return 0.0; //FIXME
}

} /* namespace viennamaterials */
