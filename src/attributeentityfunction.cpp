/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "../viennamaterials/attributeentityfunction.hpp"
#include "../viennamaterials/exceptions.hpp"

namespace viennamaterials
{

attribute_entity_function::attribute_entity_function()
{
  entity_type = invalid;
}

attribute_entity_function::attribute_entity_function(xml_attribute_type entity_type, function_backend *backend, std::vector<xml_value_entity*> &args)
{
  this->entity_type = entity_type;
  this->backend     = backend;
  this->args        = args;
}

xml_bool attribute_entity_function::eval(tag_scalar_bool tag)
{
  if(entity_type != function_bool)
    throw func_backend_attr_type_error();

  return backend->eval(tag, args);
}

xml_int attribute_entity_function::eval(tag_scalar_int tag)
{
  if(entity_type != function_int)
    throw func_backend_attr_type_error();

  return backend->eval(tag, args);
}

xml_float attribute_entity_function::eval(tag_scalar_float tag)
{
  if(entity_type != function_float)
    throw func_backend_attr_type_error();

  return backend->eval(tag, args);
}

} /* namespace viennamaterials */
