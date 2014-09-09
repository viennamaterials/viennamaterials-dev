/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/attributeentity.hpp"
#include "viennamaterials/exceptions.hpp"

namespace viennamaterials
{

bool attribute_entity::is_type(tag_scalar_bool tag)
{
  if(entity_type_ == scalar_bool)
    return true;
  return false;
}

bool attribute_entity::is_type(tag_scalar_int tag)
{
  if(entity_type_ == scalar_int)
    return true;
  return false;
}

bool attribute_entity::is_type(tag_scalar_float tag)
{
  if(entity_type_ == scalar_float)
    return true;
  return false;
}

bool attribute_entity::is_type(tag_tensor tag)
{
  if(entity_type_ == tensor)
    return true;
  return false;
}

bool attribute_entity::is_type(tag_function_bool tag)
{
  if(entity_type_ == function_bool)
    return true;
  return false;
}

bool attribute_entity::is_type(tag_function_int tag)
{
  if(entity_type_ == function_int)
    return true;
  return false;
}

bool attribute_entity::is_type(tag_function_float tag)
{
  if(entity_type_ == function_float)
    return true;
  return false;
}

bool attribute_entity::is_type(tag_function_tensor tag)
{
  if(entity_type_ == function_tensor)
    return true;
  return false;
}


bool attribute_entity::is_type(xml_attribute_type type)
{
  if(entity_type_ == type)
    return true;
  return false;
}

bool attribute_entity::is_scalar()
{
  if(     entity_type_ == scalar_bool
      ||  entity_type_ == scalar_int
      ||  entity_type_ == scalar_float)
    return true;
  return false;
}

bool attribute_entity::is_scalar(tag_scalar_bool tag)
{
  if(entity_type_ == scalar_bool)
    return true;
  return false;
}

bool attribute_entity::is_scalar(tag_scalar_int tag)
{
  if(entity_type_ == scalar_int)
    return true;
  return false;
}

bool attribute_entity::is_scalar(tag_scalar_float tag)
{
  if(entity_type_ == scalar_float)
    return true;
  return false;
}

bool attribute_entity::is_scalar_bool()
{
  if(entity_type_ == scalar_bool)
    return true;
  return false;
}

bool attribute_entity::is_scalar_int()
{
  if(entity_type_ == scalar_int)
    return true;
  return false;
}

bool attribute_entity::is_scalar_float()
{
  if(entity_type_ == scalar_float)
    return true;
  return false;
}

bool attribute_entity::is_tensor()
{
  if(entity_type_ == tensor)
    return true;
  return false;
}

bool attribute_entity::is_function()
{
  if(     entity_type_ == function_bool
      ||  entity_type_ == function_int
      ||  entity_type_ == function_float
      ||  entity_type_ == function_tensor)
    return true;
  return false;
}

bool attribute_entity::is_function(tag_function_bool tag)
{
  if(entity_type_ == function_bool)
    return true;
  return false;
}

bool attribute_entity::is_function(tag_function_int tag)
{
  if(entity_type_ == function_int)
    return true;
  return false;
}

bool attribute_entity::is_function(tag_function_float tag)
{
  if(entity_type_ == function_float)
    return true;
  return false;
}

bool attribute_entity::is_function(tag_function_tensor tag)
{
  if(entity_type_ == function_tensor)
    return true;
  return false;
}

bool attribute_entity::is_function_bool()
{
  if(entity_type_ == function_bool)
    return true;
  return false;
}

bool attribute_entity::is_function_int()
{
  if(entity_type_ == function_int)
    return true;
  return false;
}

bool attribute_entity::is_function_float()
{
  if(entity_type_ == function_float)
    return true;
  return false;
}

bool attribute_entity::is_function_tensor()
{
  if(entity_type_ == function_tensor)
    return true;
  return false;
}

std::string attribute_entity::get_unit() const
{
  return unit_;
}

template<typename T>
T attribute_entity::evaluate_value_dispatch()
{
  throw library_error("Template type not supported");
}

template<>
xml_bool attribute_entity::evaluate_value_dispatch<xml_bool>()
{
  tag_scalar_bool tag_bool;
  return this->eval_value(tag_bool);
}

template<>
xml_int attribute_entity::evaluate_value_dispatch<xml_int>()
{
  tag_scalar_int tag_int;
  return this->eval_value(tag_int);
}

template<>
xml_float attribute_entity::evaluate_value_dispatch<xml_float>()
{
  tag_scalar_float tag_float;
  return this->eval_value(tag_float);
}

} /* namespace viennamaterials */
