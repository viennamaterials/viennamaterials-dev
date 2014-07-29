/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "../viennamaterials/attributeentity.hpp"

namespace viennamaterials
{

bool viennamaterials::attribute_entity::is_scalar()
{
  if(     entity_type == scalar_bool
      ||  entity_type == scalar_int
      ||  entity_type == scalar_float)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_scalar(tag_scalar_bool tag)
{
  if(entity_type == scalar_bool)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_scalar(tag_scalar_int tag)
{
  if(entity_type == scalar_int)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_scalar(tag_scalar_float tag)
{
  if(entity_type == scalar_float)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_scalar_bool()
{
  if(entity_type == scalar_bool)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_scalar_int()
{
  if(entity_type == scalar_int)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_scalar_float()
{
  if(entity_type == scalar_float)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_tensor()
{
  if(entity_type == tensor)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_function()
{
  if(     entity_type == function_bool
      ||  entity_type == function_int
      ||  entity_type == function_float
      ||  entity_type == function_tensor)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_function(tag_function_bool tag)
{
  if(entity_type == function_bool)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_function(tag_function_int tag)
{
  if(entity_type == function_int)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_function(tag_function_float tag)
{
  if(entity_type == function_float)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_function(tag_function_tensor tag)
{
  if(entity_type == function_tensor)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_function_bool()
{
  if(entity_type == function_bool)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_function_int()
{
  if(entity_type == function_int)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_function_float()
{
  if(entity_type == function_float)
    return true;
  return false;
}

bool viennamaterials::attribute_entity::is_function_tensor()
{
  if(entity_type == function_tensor)
    return true;
  return false;
}

} /* namespace viennamaterials */
