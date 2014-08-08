/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/attributeentityscalar.hpp"
#include "viennamaterials/exceptions.hpp"

namespace viennamaterials
{

attribute_entity_scalar_boolean::attribute_entity_scalar_boolean(xml_bool value)
{
  this->entity_type_ = scalar_bool;
  this->value_ = value;
}

std::vector<xml_value_entity_handle> attribute_entity_scalar_boolean::get_dependencies()
{
  std::vector<xml_value_entity_handle> empty_vector;
  return empty_vector;
}

void attribute_entity_scalar_boolean::set_dependencies(std::vector<xml_value_entity_handle>& args)
{
  throw attribute_entity_error("Setting dependencies of scalar attribute not allowed");
}

xml_bool attribute_entity_scalar_boolean::eval(tag_scalar_bool tag)
{
  return this->value_;
}

xml_int attribute_entity_scalar_boolean::eval(tag_scalar_int tag)
{
  throw attribute_entity_error("Invalid evaluate return type for this attribute object - integer value from boolean attribute not supported");
}

xml_float attribute_entity_scalar_boolean::eval(tag_scalar_float tag)
{
  throw attribute_entity_error("Invalid evaluate return type for this attribute object - floating point value from boolean attribute not supported");
}



attribute_entity_scalar_integer::attribute_entity_scalar_integer(xml_int value)
{
  this->entity_type_ = scalar_int;
  this->value_ = value;
}

std::vector<xml_value_entity_handle> attribute_entity_scalar_integer::get_dependencies()
{
  std::vector<xml_value_entity_handle> empty_vector;
  return empty_vector;
}

void attribute_entity_scalar_integer::set_dependencies(std::vector<xml_value_entity_handle>& args)
{
  throw attribute_entity_error("Setting dependencies of scalar attribute not allowed");
}

xml_bool attribute_entity_scalar_integer::eval(tag_scalar_bool tag)
{
  throw attribute_entity_error("Invalid evaluate return type for this attribute object - boolean value from integer attribute not supported");
}

xml_int attribute_entity_scalar_integer::eval(tag_scalar_int tag)
{
  return this->value_;
}

xml_float attribute_entity_scalar_integer::eval(tag_scalar_float tag)
{
  throw attribute_entity_error("Invalid evaluate return type for this attribute object - floating point value from integer attribute not supported");
}



attribute_entity_scalar_float::attribute_entity_scalar_float(xml_float value)
{
  this->entity_type_ = scalar_float;
  this->value_ = value;
}

std::vector<xml_value_entity_handle> attribute_entity_scalar_float::get_dependencies()
{
  std::vector<xml_value_entity_handle> empty_vector;
  return empty_vector;
}

void attribute_entity_scalar_float::set_dependencies(std::vector<xml_value_entity_handle>& args)
{
  throw attribute_entity_error("Setting dependencies of scalar attribute not allowed");
}

xml_bool attribute_entity_scalar_float::eval(tag_scalar_bool tag)
{
  throw attribute_entity_error("Invalid evaluate return type for this attribute object - boolean value from floating point attribute not supported");
}

xml_int attribute_entity_scalar_float::eval(tag_scalar_int tag)
{
  throw attribute_entity_error("Invalid evaluate return type for this attribute object - integer value from floating point attribute not supported");
}

xml_float attribute_entity_scalar_float::eval(tag_scalar_float tag)
{
  return this->value_;
}

} /* namespace viennamaterials */