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

/**
 * @brief Provides a Python function backend based on the function_backend interface for an attribute_entity_function object
 */
class function_backend_python : public function_backend
{
public:
            ~function_backend_python();

  /**
   * @brief Initialize the function backend
   * @param code The backend specific code which provides the functionality
   * @param function_name The name of the function to be called within the given code
   */
  void      init(std::string const& code, std::string const& function_name);

  /**
   * @brief Executes the function specified during initialization
   * @param tag A Tag of type tag_scalar_bool
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   * @return A xml_bool value representing the result of the executed function
   */
  xml_bool  eval(tag_scalar_bool tag,   std::vector<xml_value_entity_handle>& args);

  /**
   * @brief Executes the function specified during initialization
   * @param tag A Tag of type tag_scalar_int
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   * @return A xml_int value representing the result of the executed function
   */
  xml_int   eval(tag_scalar_int tag,    std::vector<xml_value_entity_handle>& args);

  /**
   * @brief Executes the function specified during initialization
   * @param tag A Tag of type tag_scalar_float
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   * @return A xml_float value representing the result of the executed function
   */
  xml_float eval(tag_scalar_float tag,  std::vector<xml_value_entity_handle>& args);

private:
  PyObject  *function_ptr_; /// Pointer to function in python module object
  PyObject  *global_ptr_;
  PyObject  *local_ptr_;
  PyObject  *module_ptr_;

  /**
   * @brief Executes the function specified during initialization
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   * @return A pointer to a PyObject representing the result of the executed function
   */
  PyObject* eval(std::vector<xml_value_entity_handle>& args);
};

} /* namespace viennamaterials */
#endif /* FUNCTIONBACKENDPYTHON_HPP_ */
