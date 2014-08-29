/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/functionbackendpython.hpp"
#include "viennamaterials/exceptions.hpp"

namespace viennamaterials
{

void function_backend_python::init(std::string const& code, std::string const& function_name)
{
  /// Initialize the Python Interpreter
  Py_Initialize();

  global_ptr_ = PyDict_New();

  /// Create a new module object
  module_ptr_ = PyModule_New("mymod");
  PyModule_AddStringConstant(module_ptr_, "__file__", "");

  /// Get the dictionary object from my module so it can be passed to PyRun_String
  local_ptr_ = PyModule_GetDict(module_ptr_);

  /// Define the function in the newly created module
  PyRun_String(code.c_str(), Py_file_input, global_ptr_, local_ptr_);

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

PyObject* function_backend_python::eval(std::vector<xml_value_entity_handle>& args)
{
  if (PyCallable_Check(function_ptr_))
  {
    size_t number_of_args = args.size();

    PyObject *result_py;
    /// Build a tuple to hold arguments
    PyObject *args_py;
    args_py = PyTuple_New(number_of_args);
    PyObject *value_py = NULL;
    for(std::vector<xml_value_entity_handle>::iterator it = args.begin(); it != args.end(); ++it)
    {
      switch((*it)->get_type())
      {
        case type_bool:
          value_py = PyBool_FromLong((*it)->get_value<xml_bool>());
          break;
        case type_int:
          value_py = PyInt_FromLong((*it)->get_value<xml_int>());
          break;
        case type_float:
          value_py = PyFloat_FromDouble((*it)->get_value<xml_float>());
          break;
        case type_tensor:
          //TODO tensor
          break;
        default:
          throw func_backend_error("Not supported xml_value_entity_type encountered");
          break;
      }

      PyTuple_SetItem(args_py, (*it)->get_index(), value_py);
    }

    result_py = PyObject_CallObject(function_ptr_, args_py);

    if(value_py != NULL)
      Py_DECREF(value_py);
    Py_DECREF(args_py);

    if(result_py == NULL)
      throw func_backend_error("Failure in Python function");
    return result_py;
  } else
  {
    PyErr_Print();
  }

  throw func_backend_python_no_function_error();
}

xml_bool function_backend_python::eval(tag_scalar_bool tag, std::vector<xml_value_entity_handle>& args)
{
  PyObject *result_py = eval(args);
  xml_bool result = false;
  if(PyObject_IsTrue(result_py))
    result = true;
  Py_DECREF(result_py);

  return result;
}

xml_int function_backend_python::eval(tag_scalar_int tag, std::vector<xml_value_entity_handle>& args)
{
  PyObject *result_py = eval(args);
  xml_int result = PyInt_AsLong(result_py);
  Py_DECREF(result_py);

  return result;
}

xml_float function_backend_python::eval(tag_scalar_float tag, std::vector<xml_value_entity_handle>& args)
{
  PyObject *result_py = eval(args);
  xml_float result = PyFloat_AsDouble(result_py);
  Py_DECREF(result_py);

  return result;
}

} /* namespace viennamaterials */
