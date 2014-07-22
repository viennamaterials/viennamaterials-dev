/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "../viennamaterials/attributefunctionpython.h"

namespace viennamaterials
{

std::vector<FunctionArgumentBase> attribute_function_python::init(viennamaterials::library_handle& lib, std::string& xpath_query)
{

  /* TODO
   * get number of args
   * get info of args (quant, type, default)
   * get type of result
   *
   *
   * return list of arguments + result type
   */

  // Initialize the Python Interpreter
  Py_Initialize();

  PyObject *pValue;
  pGlobal = PyDict_New();

  //Create a new module object
  pNewMod = PyModule_New("mymod");
  PyModule_AddStringConstant(pNewMod, "__file__", "");

  //Get the dictionary object from my module so it can be passed to PyRun_String
  pLocal = PyModule_GetDict(pNewMod);

  //Define the function in the newly created module
  std::string python_function_string = lib->query(xpath_query + "/code/text()");
  pValue = PyRun_String(python_function_string.c_str(), Py_file_input, pGlobal, pLocal);
  Py_DECREF(pValue);

  //Query number of arguments
  number_of_arguments = query_number_of_arguments(lib, xpath_query);

  //Query function name
  std::string func_name = lib->query_xpath_string("string(" + xpath_query + "/code/@call)");

  //Get a pointer to the function
  pFunc = PyObject_GetAttrString(pNewMod, func_name.c_str());



  //FIXME
  std::vector<FunctionArgumentBase> empty_vector;
  return empty_vector;

}

void attribute_function_python::deinit()
{
  Py_XDECREF(pFunc);
  Py_DECREF(pGlobal);
  Py_DECREF(pLocal);
  Py_DECREF(pNewMod);

  // Finish the Python Interpreter
  Py_Finalize();
}

FunctionArgumentBase attribute_function_python::evaluate(std::vector<FunctionArgumentBase> args)
{

  /* TODO
   * wrap arguments into PyObjects
   *
   * wrap result into FunctionArgument
   * return FunctionArgument
   */

  if (PyCallable_Check(pFunc))
  {
    //Build a tuple to hold arguments
    PyObject *pArgs;
    pArgs = PyTuple_New((Py_ssize_t) 2);
    PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(3.0));
    PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(0.5));

    PyObject *pValue;
    pValue = PyObject_CallObject(pFunc, pArgs);
    std::cout << "return: " << PyFloat_AsDouble(pValue) << std::endl;

    Py_DECREF(pValue);
    Py_DECREF(pArgs);
  } else
  {
    PyErr_Print();
  }







  FunctionArgumentBase dummy;
  return dummy;
}

} /* namespace viennamaterials */
