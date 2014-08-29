/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef XMLVALUEENTITY_HPP_
#define XMLVALUEENTITY_HPP_

#include <string>
#include "viennamaterials/xmldatatypes.h"

namespace viennamaterials
{

enum xml_value_entity_types
{
  type_bool,
  type_int,
  type_float,
  type_tensor
};
typedef enum xml_value_entity_types  xml_value_entity_type;


/**
 * @brief Interface class for XML scalar and tensor elements which are used as arguments for attribute entity functions
 */
class xml_value_entity
{
public:
  virtual               ~xml_value_entity() {};

  /**
   * @brief Set name of this object
   * @param name A string containing the name
   */
  void                  set_name(std::string const& name);

  /**
   * @brief Get name of this object
   * @return A string containing the name
   */
  std::string           get_name() const;

  /**
   * @brief Get type of this object
   * @return The type of the object represented by enum xml_type
   */
  xml_value_entity_type get_type();

  /**
   * @brief Set index of this object. The index is the position in the argument list of a function.
   * @param index The index of this object
   */
  void                  set_index(size_t index);

  /**
   * @brief Get index of this object. The index is the position in the argument list of a function.
   * @return The index of this object
   */
  size_t                get_index();

  /**
   * @brief Set bool value of this object
   * @param value A bool value
   */
  virtual void          set_value(xml_bool value)    = 0;

  /**
   * @brief Set integer value of this object
   * @param value A integer value
   */
  virtual void          set_value(xml_int value)     = 0;
  /**
   * @brief Set floating point value of this object
   * @param value A floating point value
   */
  virtual void          set_value(xml_float value)   = 0;

  /**
   * @brief Set tensor value of this object
   * @param value A tensor object
   */
//  virtual void           set_value(void value)             = 0; //TODO tensor: arg

  /**
   * @brief Get value of this object
   * @tparam Type of the value of this object
   * @return The value of this object according to the template type
   */
  template<typename T>
  T get_value();

protected:

  /**
   * @brief Dispatcher for the template specializations of get_value method
   * @tparam T Type of the value of this object
   * @return The value of this object according to the template type
   */
  template<typename T>
  T get_value_dispatch();

  /**
   * @brief Get bool value of this object
   * @param tag A tag of type tag_scalar_bool
   * @return The bool value of this object
   */
  virtual xml_bool  get_value(tag_scalar_bool tag)  = 0;

  /**
   * @brief Get integer value of this object
   * @param tag A tag of type tag_scalar_int
   * @return The integer value of this object
   */
  virtual xml_int   get_value(tag_scalar_int tag)   = 0;

  /**
   * @brief Get floating point value of this object
   * @param tag A tag of type tag_scalar_float
   * @return The floating point value of this object
   */
  virtual xml_float get_value(tag_scalar_float tag) = 0;

  /**
   * @brief Get tensor object of this object
   * @param tag A tag of type tag_tensor
   * @return The tensor object of this object
   */
//  virtual void      get_value(tag_tensor tag)       = 0; //TODO tensor: return

protected:
  /**
   * @brief Set type of this value object
   * @param type The type given as enum xml_type
   */
  void              set_type(xml_value_entity_type type);

private:
  size_t                index_; /// Argument index for the function backend
  std::string           name_;  /// Used as identifier for simulator
  xml_value_entity_type type_;  /// XML type of this value
};

template<typename T>
T xml_value_entity::get_value()
{
  return this->get_value_dispatch<T>();
}

typedef shared_ptr<xml_value_entity> xml_value_entity_handle;

} /* namespace viennamaterials */
#endif /* XMLVALUEENTITY_HPP_ */
