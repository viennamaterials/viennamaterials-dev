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

void xml_value_entity::set_name(std::string name)
{
  this->name = name;
}

std::string xml_value_entity::get_name() const
{
  return name;
}

void xml_value_entity::set_type(xml_type type)
{
  this->type = type;
}

xml_type xml_value_entity::get_type()
{
  return type;
}


} /* namespace viennamaterials */
