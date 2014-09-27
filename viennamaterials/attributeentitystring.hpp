/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef ATTRIBUTEENTITYSTRING_HPP_
#define ATTRIBUTEENTITYSTRING_HPP_

#include "attributeentity.hpp"

namespace viennamaterials
{

/**
 * @brief Represents an attribute XML element holding a string
 */
class attribute_entity_string : public viennamaterials::attribute_entity
{
public:
  /**
   * @brief Create a string attribute
   * @param value The value of this attribute object
   * @param unit The unit of this attribute object
   */
  attribute_entity_string(xml_string const& value, std::string const& unit);

  ~attribute_entity_string();

  /**
   * @brief Get the dependencies of the scalar attribute. Since a scalar has no dependencies, an empty vector is returned.
   * @return An empty vector of smartpointers to objects of type xml_value_entity
   */
  std::vector<xml_value_entity_handle>  get_dependencies();


  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  void                                  set_dependencies(std::vector<xml_value_entity_handle>& args);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_bool                              eval_value(tag_scalar_bool tag);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_int                               eval_value(tag_scalar_int tag);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_float                             eval_value(tag_scalar_float tag);

  /**
   * @brief Get the value of this string attribute
   * @param tag A tag of type tag_string
   * @return The string value of this attribute object
   */
  xml_string                            eval_value(tag_string tag);

private:
  xml_string value_;
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITYSTRING_HPP_ */
