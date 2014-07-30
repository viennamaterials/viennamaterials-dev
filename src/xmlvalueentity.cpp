/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "../viennamaterials/xmlvalueentity.hpp"

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

void xml_value_entity::set_type(xml_type type)
{
  this->type_ = type;
}

xml_type xml_value_entity::get_type()
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

} /* namespace viennamaterials */
