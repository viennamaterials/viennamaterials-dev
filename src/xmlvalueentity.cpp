/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/xmlvalueentity.hpp"
#include "viennamaterials/exceptions.hpp"

namespace viennamaterials
{

void xml_value_entity::set_name(std::string const& name)
{
  this->name_ = name;
}

std::string xml_value_entity::get_name() const
{
  return name_;
}

void xml_value_entity::set_type(xml_value_entity_type type)
{
  this->type_ = type;
}

xml_value_entity_type xml_value_entity::get_type()
{
  return type_;
}

void xml_value_entity::set_index(size_t index)
{
  this->index_ = index;
}

size_t xml_value_entity::get_index()
{
  return index_;
}

template<typename T>
T xml_value_entity::get_value_dispatch()
{
  throw library_error("Template type not supported");
}

template<>
xml_bool xml_value_entity::get_value_dispatch<xml_bool>()
{
  tag_scalar_bool tag_bool;
  return this->get_value(tag_bool);
}

template<>
xml_int xml_value_entity::get_value_dispatch<xml_int>()
{
  tag_scalar_int tag_int;
  return this->get_value(tag_int);
}

template<>
xml_float xml_value_entity::get_value_dispatch<xml_float>()
{
  tag_scalar_float tag_float;
  return this->get_value(tag_float);
}

} /* namespace viennamaterials */
