/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef XMLVALUESCALARINTEGER_HPP_
#define XMLVALUESCALARINTEGER_HPP_

#include "xmlvalueentity.hpp"
#include "xmldatatypes.h"

namespace viennamaterials
{

/**
 * @brief Represents a XML scalar element of type integer
 */
class xml_value_scalar_integer : public xml_value_entity
{
public:
  xml_value_scalar_integer();


  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  void      set_value(xml_bool value);

  /**
   * @brief Set integer value of this object
   * @param value A integer value
   */
  void      set_value(xml_int value);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  void      set_value(xml_float value);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_bool  get_value(tag_scalar_bool tag);

  /**
   * @brief Get integer value of this object
   * @return A integer value
   */
  xml_int   get_value(tag_scalar_int tag);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_float get_value(tag_scalar_float tag);

private:
  xml_int value_;
};

/**
 * @brief Represents a XML scalar element of type float
 */
class xml_value_scalar_float : public xml_value_entity
{
public:
  xml_value_scalar_float();

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  void      set_value(xml_bool value);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  void      set_value(xml_int value);

  /**
   * @brief Set floating point value of this object
   * @param value A floating point value
   */
  void      set_value(xml_float value);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_bool  get_value(tag_scalar_bool tag);

  /// @brief Implements a pure virtual method of its base class. This methods throws an exception!
  xml_int   get_value(tag_scalar_int tag);

  /**
   * @brief Get floating point value of this object
   * @return A floating point value
   */
  xml_float get_value(tag_scalar_float tag);

private:
  xml_float value;
};

} /* namespace viennamaterials */
#endif /* XMLVALUESCALARINTEGER_HPP_ */
