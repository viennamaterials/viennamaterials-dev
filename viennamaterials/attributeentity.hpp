/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef ATTRIBUTEENTITY_HPP_
#define ATTRIBUTEENTITY_HPP_

#include "viennamaterials/xmldatatypes.h"
#include "viennamaterials/xmlvalueentity.hpp"
#include <vector>

namespace viennamaterials
{

/**
 * @brief Represents an attribute XML element
 */
class attribute_entity
{
public:
  virtual ~attribute_entity() {};

  //TODO: DOXIGENIZE
  bool is_type(tag_scalar_bool tag);
  //TODO etc
  bool is_type(xml_attribute_type type);

  bool is_scalar();
  bool is_scalar(tag_scalar_bool tag);
  bool is_scalar(tag_scalar_int tag);
  bool is_scalar(tag_scalar_float tag);
  bool is_scalar_bool();
  bool is_scalar_int();
  bool is_scalar_float();
  bool is_tensor();
  bool is_function();
  bool is_function(tag_function_bool tag);
  bool is_function(tag_function_int tag);
  bool is_function(tag_function_float tag);
  bool is_function(tag_function_tensor tag);
  bool is_function_bool();
  bool is_function_int();
  bool is_function_float();
  bool is_function_tensor();

  /**
   * @brief Get the dependencies of the attribute
   * @return A vector of smartpointers to objects of type xml_value_entity
   */
  virtual std::vector<xml_value_entity_handle>  get_dependencies() = 0;

  /**
   * @brief Set the dependencies of the attribute
   * @param A vector of smartpointers to objects of type xml_value_entity
   */
  virtual void                                  set_dependencies(std::vector<xml_value_entity_handle>& args) = 0;

  /**
   * @brief Evaluate the value of the attribute
   * @tparam T Type of the attribute value
   * @return The value of the attribute according to the template type
   */
  template<typename T>
  T evaluate();

protected:

  /**
   * @brief Dispatcher for the template specializations of evaluate method
   * @tparam T Type of the attribute value
   * @return The value of the attribute according to the template type
   */
  template<typename T>
  T evaluate_dispatch();

  /**
   * @brief Evaluate the value of this attribute
   * @param tag A tag of type tag_scalar_bool
   * @return The bool value of the attribute
   */
  virtual xml_bool  eval(tag_scalar_bool tag)     = 0;

  /**
   * @brief Evaluate the value of this attribute
   * @param tag A tag of type tag_scalar_int
   * @return The integer value of the attribute
   */
  virtual xml_int   eval(tag_scalar_int tag)      = 0;

  /**
   * @brief Evaluate the value of this attribute
   * @param tag A tag of type tag_scalar_float
   * @return The floating point value of the attribute
   */
  virtual xml_float eval(tag_scalar_float tag)    = 0;

  /**
   * @brief Evaluate the value of this attribute
   * @param tag A tag of type tag_tensor
   * @return The tensor object of the attribute
   */
//  virtual void      eval(tag_tensor tag)          = 0; //FIXME: return

protected:
  xml_attribute_type entity_type_;
  //TODO unit!!!
};

template<typename T>
T attribute_entity::evaluate()
{
  return this->evaluate_dispatch<T>();
}

} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITY_HPP_ */
