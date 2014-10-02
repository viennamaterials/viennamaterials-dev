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
#include "viennamaterials/quantity.hpp"
#include <vector>

namespace viennamaterials
{

/**
 * @brief Represents an attribute XML element. Used as interface class for specialized attributes.
 */
class attribute_entity
{
public:
  virtual ~attribute_entity() {};

  /**
   * @brief Check if this object represents a boolean scalar
   * @param tag A tag of type tag_scalar_bool (tag specialization)
   * @return True if this object represents a boolean scalar, false otherwise
   */
  bool is_type(tag_scalar_bool tag);

  /**
   * @brief Check if this object represents an integer scalar
   * @param tag A tag of type tag_scalar_int (tag specialization)
   * @return True if this object represents an integer scalar, false otherwise
   */
  bool is_type(tag_scalar_int tag);

  /**
   * @brief Check if this object represents a floating point scalar
   * @param tag A tag of type tag_scalar_float (tag specialization)
   * @return True if this object represents a floating point scalar, false otherwise
   */
  bool is_type(tag_scalar_float tag);

  /**
   * @brief Check if this object represents a tensor
   * @param tag A tag of type tag_tensor (tag specialization)
   * @return True if this object represents a tensor, false otherwise
   */
  bool is_type(tag_tensor tag);

  /**
   * @brief Check if this object represents a function attribute with boolean result
   * @param tag A tag of type tag_function_bool (tag specialization)
   * @return True if this object represents a function attribute with boolean result, false otherwise
   */
  bool is_type(tag_function_bool tag);

  /**
   * @brief Check if this object represents a function attribute with integer result
   * @param tag A tag of type tag_function_int (tag specialization)
   * @return True if this object represents a function attribute with integer result, false otherwise
   */
  bool is_type(tag_function_int tag);

  /**
   * @brief Check if this object represents a function attribute with floating point result
   * @param tag A tag of type tag_function_float (tag specialization)
   * @return True if this object represents a function attribute with floating point result, false otherwise
   */
  bool is_type(tag_function_float tag);

  /**
   * @brief Check if this object represents a function attribute with tensor result
   * @param tag A tag of type tag_function_tensor (tag specialization)
   * @return True if this object represents a function attribute with tensor result, false otherwise
   */
  bool is_type(tag_function_tensor tag);

  /**
   * @brief Check if this object represents an attribute of given type
   * @param type The type to be checked given as xml_attribute_type
   * @return True if this object is of given type, false otherwise
   */
  bool is_type(xml_attribute_type type);


  /**
   * @brief Check if this object represents a scalar
   * @return True if this object represents a scalar, false otherwise
   */
  bool is_scalar();

  /**
   * @brief Check if this object represents a boolean scalar
   * @param tag A tag of type tag_scalar_bool (tag specialization)
   * @return True if this object represents a boolean scalar, false otherwise
   */
  bool is_scalar(tag_scalar_bool tag);

  /**
   * @brief Check if this object represents a integer scalar
   * @param tag A tag of type tag_scalar_int (tag specialization)
   * @return True if this object represents a integer scalar, false otherwise
   */
  bool is_scalar(tag_scalar_int tag);

  /**
   * @brief Check if this object represents a floating point scalar
   * @param tag A tag of type tag_scalar_float (tag specialization)
   * @return True if this object represents a floating point scalar, false otherwise
   */
  bool is_scalar(tag_scalar_float tag);

  /**
   * @brief Check if this object represents a boolean scalar
   * @return True if this object represents a boolean scalar, false otherwise
   */
  bool is_scalar_bool();

  /**
   * @brief Check if this object represents a integer scalar
   * @return True if this object represents a integer scalar, false otherwise
   */
  bool is_scalar_int();

  /**
   * @brief Check if this object represents a floating point scalar
   * @return True if this object represents a floating point scalar, false otherwise
   */
  bool is_scalar_float();

  /**
   * @brief Check if this object represents a tensor
   * @return True if this object represents a tensor, false otherwise
   */
  bool is_tensor();

  /**
   * @brief Check if this object represents a function
   * @return True if this object represents a function, false otherwise
   */
  bool is_function();

  /**
   * @brief Check if this object represents a function attribute with boolean result
   * @param tag A tag of type tag_function_bool (tag specialization)
   * @return True if this object represents a function attribute with boolean result, false otherwise
   */
  bool is_function(tag_function_bool tag);

  /**
   * @brief Check if this object represents a function attribute with integer result
   * @param tag A tag of type tag_function_int (tag specialization)
   * @return True if this object represents a function attribute with integer result, false otherwise
   */
  bool is_function(tag_function_int tag);

  /**
   * @brief Check if this object represents a function attribute with floating point result
   * @param tag A tag of type tag_function_float (tag specialization)
   * @return True if this object represents a function attribute with floating point result, false otherwise
   */
  bool is_function(tag_function_float tag);

  /**
   * @brief Check if this object represents a function attribute with tensor result
   * @param tag A tag of type tag_function_tensor (tag specialization)
   * @return True if this object represents a function attribute with tensor result, false otherwise
   */
  bool is_function(tag_function_tensor tag);

  /**
   * @brief Check if this object represents a function attribute with boolean result
   * @return True if this object represents a function attribute with boolean result, false otherwise
   */
  bool is_function_bool();

  /**
   * @brief Check if this object represents a function attribute with integer result
   * @return True if this object represents a function attribute with integer result, false otherwise
   */
  bool is_function_int();

  /**
   * @brief Check if this object represents a function attribute with floating point result
   * @return True if this object represents a function attribute with floating point result, false otherwise
   */
  bool is_function_float();

  /**
   * @brief Check if this object represents a function attribute with tensor result
   * @return True if this object represents a function attribute with tensor result, false otherwise
   */
  bool is_function_tensor();

  /**
   * @brief Get unit of this object
   * @return A string containing the unit
   */
  std::string           get_unit() const;

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
  T evaluate_value();

  /**
   * @brief Evaluate the quantity of the attribute
   * @tparam T Type of the attribute value
   * @return The quantity of the attribute
   */
  template<typename T>
  quantity<T> evaluate();

protected:

  /**
   * @brief Dispatcher for the template specializations of evaluate_value method
   * @tparam T Type of the attribute value
   * @return The value of the attribute according to the template type
   */
  template<typename T>
  T evaluate_value_dispatch();

  /**
   * @brief Dispatcher for the template specializations of evaluate method
   * @tparam T Type of the attribute value
   * @return The quantity of the attribute
   */
  template<typename T>
  quantity<T> evaluate_dispatch();

  /**
   * @brief Evaluate the value of this attribute
   * @param tag A tag of type tag_scalar_bool
   * @return The boolean value of the attribute
   */
  virtual xml_bool  eval_value(tag_scalar_bool tag)     = 0;

  /**
   * @brief Evaluate the value of this attribute
   * @param tag A tag of type tag_scalar_int
   * @return The integer value of the attribute
   */
  virtual xml_int   eval_value(tag_scalar_int tag)      = 0;

  /**
   * @brief Evaluate the value of this attribute
   * @param tag A tag of type tag_scalar_float
   * @return The floating point value of the attribute
   */
  virtual xml_float eval_value(tag_scalar_float tag)    = 0;

  /**
   * @brief Evaluate the value of this attribute
   * @param tag A tag of type tag_tensor
   * @return The tensor object of the attribute
   */
//  virtual void      eval_value(tag_tensor tag)          = 0; //TODO: tensor: return

protected:
  xml_attribute_type  entity_type_;
  std::string         unit_;
};

template<typename T>
T attribute_entity::evaluate_value()
{
  return this->evaluate_value_dispatch<T>();
}

template<typename T>
quantity<T> attribute_entity::evaluate()
{
  return this->evaluate_dispatch<T>();
}

template<typename T>
quantity<T> attribute_entity::evaluate_dispatch()
{
  quantity<T> result(this->evaluate_value_dispatch<T>(), unit_);
  return result;
}



} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITY_HPP_ */
