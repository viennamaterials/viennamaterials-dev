/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef FUNCTIONBACKENDPYTHON_HPP_
#define FUNCTIONBACKENDPYTHON_HPP_

#include "viennamaterials/functionbackend.hpp"
#include <Python.h>

namespace viennamaterials
{

class function_backend_python : public function_backend
{
public:
            ~function_backend_python();
  void      init(std::string code, std::string function_name);
  xml_bool  eval(tag_scalar_bool tag); //FIXME: argument list
  xml_int   eval(tag_scalar_int tag); //FIXME: argument list
  xml_float eval(tag_scalar_float tag); //FIXME: argument list

private:
  PyObject *function_ptr_; /// Pointer to function in python module object
  PyObject *global_ptr_;
  PyObject *local_ptr_;
  PyObject *module_ptr_;

  double    eval(); //XXX testing only
};

} /* namespace viennamaterials */
#endif /* FUNCTIONBACKENDPYTHON_HPP_ */
