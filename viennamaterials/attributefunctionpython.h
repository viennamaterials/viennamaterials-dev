/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef ATTRIBUTEFUNCTIONPYTHON_H_
#define ATTRIBUTEFUNCTIONPYTHON_H_

#include "viennamaterials/attributefunction.h"
#include <Python.h>

namespace viennamaterials
{

class attribute_function_python : public IAttributeFunction
{
  std::vector<FunctionArgumentBase>   init(viennamaterials::library_handle& lib, std::string& xpath_query);
  void                                deinit();
  FunctionArgumentBase                evaluate(std::vector<FunctionArgumentBase> args);

private:
  long number_of_arguments;
  PyObject *pFunc; //pointer to function in python module object
  PyObject *pGlobal;
  PyObject *pLocal;
  PyObject *pNewMod;
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEFUNCTIONPYTHON_H_ */
