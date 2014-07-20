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
   * init python
   * load code
   *
   * return list of arguments + result type
   */

  std::vector<FunctionArgumentBase> empty_vector;
  return empty_vector;

}

void attribute_function_python::deinit()
{
  //TODO: deinit python (free PyObjects)
}

FunctionArgumentBase attribute_function_python::evaluate(std::vector<FunctionArgumentBase> args)
{

  /* TODO
   * wrap arguments into PyObjects
   * call python function
   * wrap result into FunctionArgument
   * return FunctionArgument
   */

  FunctionArgumentBase dummy;
  return dummy;
}

} /* namespace viennamaterials */
