/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef ATTRIBUTEENTITYSCALAR_HPP_
#define ATTRIBUTEENTITYSCALAR_HPP_

#include "viennamaterials/attributeentity.hpp"

namespace viennamaterials
{

/**
 * @brief Represents an attribute XML element holding a scalar
 */
class attribute_entity_scalar_boolean : public attribute_entity
{
public:
  /**
   * @brief Create a boolean scalar attribute
   * @param value The value of this attribute object
   */
  attribute_entity_scalar_boolean(xml_bool value);

  /**
   * @brief Get the dependencies of the scalar attribute. Since a scalar has no dependencies, an empty vector is returned.
   * @return An empty vector of smartpointers to objects of type xml_value_entity
   */
  std::vector<xml_value_entity_handle>  get_dependencies();


  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  void                                  set_dependencies(std::vector<xml_value_entity_handle>& args);

  /**
   * @brief Get the value of this boolean scalar attribute
   * @param tag A tag of type tag_scalar_bool
   * @return The boolean value of this attribute object
   */
  xml_bool                              eval(tag_scalar_bool tag);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_int                               eval(tag_scalar_int tag);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_float                             eval(tag_scalar_float tag);

private:
  xml_bool value_;
};



/**
 * @brief Represents an attribute XML element holding an integer
 */
class attribute_entity_scalar_integer: public attribute_entity
{
public:
  /**
   * @brief Create an interger scalar attribute
   * @param value The value of this attribute object
   */
  attribute_entity_scalar_integer(xml_int value);

  /**
   * @brief Get the dependencies of the scalar attribute. Since a scalar has no dependencies, an empty vector is returned.
   * @return An empty vector of smartpointers to objects of type xml_value_entity
   */
  std::vector<xml_value_entity_handle>  get_dependencies();


  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  void                                  set_dependencies(std::vector<xml_value_entity_handle>& args);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_bool                              eval(tag_scalar_bool tag);

  /**
   * @brief Get the value of this integer scalar attribute
   * @param tag A tag of type tag_scalar_int
   * @return The integer value of this attribute object
   */
  xml_int                               eval(tag_scalar_int tag);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_float                             eval(tag_scalar_float tag);

private:
  xml_int value_;
};



/**
 * @brief Represents an attribute XML element holding a floating point number
 */
class attribute_entity_scalar_float: public attribute_entity
{
public:
  /**
   * @brief Create an floating point scalar attribute
   * @param value The value of this attribute object
   */
  attribute_entity_scalar_float(xml_float value);

  /**
   * @brief Get the dependencies of the scalar attribute. Since a scalar has no dependencies, an empty vector is returned.
   * @return An empty vector of smartpointers to objects of type xml_value_entity
   */
  std::vector<xml_value_entity_handle>  get_dependencies();


  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  void                                  set_dependencies(std::vector<xml_value_entity_handle>& args);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_bool                              eval(tag_scalar_bool tag);

  /**
   * @brief Get the value of this integer scalar attribute
   * @param tag A tag of type tag_scalar_int
   * @return The integer value of this attribute object
   */
  xml_int                               eval(tag_scalar_int tag);

  /**
   * @brief Get the value of this floating point scalar attribute
   * @param tag A tag of type tag_scalar_float
   * @return The floating point value of this attribute object
   */
  xml_float                             eval(tag_scalar_float tag);

private:
  xml_float value_;
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITYSCALAR_HPP_ */
