/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef FUNCTIONBACKEND_HPP_
#define FUNCTIONBACKEND_HPP_

#include <string>
#include <vector>
#include "viennamaterials/xmldatatypes.h"
#include "viennamaterials/xmlvalueentity.hpp"

namespace viennamaterials
{

/**
 * @brief Interface class for function backends
 */
class function_backend
{
public:
  virtual           ~function_backend() {};

  /**
   * @brief Initialize the function backend
   * @param code The backend specific code which provides the functionality
   * @param function_name The name of the function to be called within the given code
   */
  virtual void      init(std::string const& code, std::string const& function_name) = 0;

  /**
   * @brief Executes the function specified during initialization
   * @param tag A Tag of type tag_scalar_bool
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   * @return A xml_bool value representing the result of the executed function
   */
  virtual xml_bool  eval(tag_scalar_bool tag,   std::vector<xml_value_entity_handle>& args)  = 0;

  /**
   * @brief Executes the function specified during initialization
   * @param tag A Tag of type tag_scalar_int
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   * @return A xml_int value representing the result of the executed function
   */
  virtual xml_int   eval(tag_scalar_int tag,    std::vector<xml_value_entity_handle>& args)  = 0;

  /**
   * @brief Executes the function specified during initialization
   * @param tag A Tag of type tag_scalar_float
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   * @return A xml_float value representing the result of the executed function
   */
  virtual xml_float eval(tag_scalar_float tag,  std::vector<xml_value_entity_handle>& args)  = 0;

  /**
   * @brief Executes the function specified during initialization
   * @param tag A Tag of type tag_tensor
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   * @return A xml_tensor value representing the result of the executed function
   */
//  virtual void      evaluate(tag_tensor) = 0; //TODO: tensor: argument list, return
};

} /* namespace viennamaterials */
#endif /* FUNCTIONBACKEND_HPP_ */
