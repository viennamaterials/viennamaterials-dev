/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/attributeentitystring.hpp"
#include "viennamaterials/exceptions.hpp"

namespace viennamaterials
{

attribute_entity_string::attribute_entity_string(xml_string const& value, std::string const& unit)
{
  this->entity_type_  = string;
  this->value_        = value;
  this->unit_         = unit;
}

attribute_entity_string::~attribute_entity_string()
{
}

std::vector<xml_value_entity_handle> attribute_entity_string::get_dependencies()
{
  std::vector<xml_value_entity_handle> empty_vector;
  return empty_vector;
}

void attribute_entity_string::set_dependencies(std::vector<xml_value_entity_handle>& args)
{
  throw attribute_entity_error("Setting dependencies of string attribute not allowed");
}

xml_bool attribute_entity_string::eval_value(tag_scalar_bool tag)
{
  throw attribute_entity_error("Invalid evaluate return type for this attribute object - boolean value from string attribute not supported");
}

xml_int attribute_entity_string::eval_value(tag_scalar_int tag)
{
  throw attribute_entity_error("Invalid evaluate return type for this attribute object - integer value from string attribute not supported");
}

xml_float attribute_entity_string::eval_value(tag_scalar_float tag)
{
  throw attribute_entity_error("Invalid evaluate return type for this attribute object - floating point value from string attribute not supported");
}

xml_string attribute_entity_string::eval_value(tag_string tag)
{
  return this->value_;
}

} /* namespace viennamaterials */
