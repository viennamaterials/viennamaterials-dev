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

attribute_entity_function::attribute_entity_function(xml_attribute_type entity_type, function_backend *backend, std::vector<xml_value_entity*> &args)
{
  this->entity_type_ = entity_type;
  this->backend_     = backend;
  this->args_        = args;
}

xml_bool attribute_entity_function::eval(tag_scalar_bool tag)
{
  if(entity_type_ != function_bool)
    throw func_backend_attr_type_error();

  return backend_->eval(tag, args_);
}

xml_int attribute_entity_function::eval(tag_scalar_int tag)
{
  if(entity_type_ != function_int)
    throw func_backend_attr_type_error();

  return backend_->eval(tag, args_);
}

xml_float attribute_entity_function::eval(tag_scalar_float tag)
{
  if(entity_type_ != function_float)
    throw func_backend_attr_type_error();

  return backend_->eval(tag, args_);
}

} /* namespace viennamaterials */
