/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/attributeentityfunction.hpp"
#include "viennamaterials/exceptions.hpp"
#include <algorithm>

namespace viennamaterials
{

attribute_entity_function::attribute_entity_function(xml_attribute_type entity_type, function_backend_handle backend, std::vector<xml_value_entity_handle>& args)
{
  this->entity_type_ = entity_type;
  this->backend_     = backend;
  this->args_        = args;
}

std::vector<xml_value_entity_handle> attribute_entity_function::get_dependencies()
{
  return args_;
}

void attribute_entity_function::set_dependencies(std::vector<xml_value_entity_handle>& args)
{
  for(std::vector<xml_value_entity_handle>::iterator function_arg_iter = args_.begin(); function_arg_iter != args_.end(); ++function_arg_iter)
  {
    std::string name = (*function_arg_iter)->get_name();
    std::vector<xml_value_entity_handle>::iterator passed_arg_iter = std::find_if(args.begin(), args.end(), arg_comperator(name));

    switch((*passed_arg_iter)->get_type())
    {
      case scalar_bool:
        (*function_arg_iter)->set_value((*passed_arg_iter)->get_value<xml_bool>());
        break;
      case scalar_int:
        (*function_arg_iter)->set_value((*passed_arg_iter)->get_value<xml_int>());
        break;
      case scalar_float:
        (*function_arg_iter)->set_value((*passed_arg_iter)->get_value<xml_float>());
        break;
      default:
        throw attribute_entity_error("Invalid argument type encountered");
    }
  }
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
