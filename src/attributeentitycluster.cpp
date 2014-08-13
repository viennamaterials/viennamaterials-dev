/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/attributeentitycluster.hpp"
#include "viennamaterials/exceptions.hpp"
#include "viennamaterials/xmlvaluescalar.hpp"

namespace viennamaterials
{

attribute_entity_cluster::attribute_entity_cluster(
    xml_attribute_type entity_type, function_backend_handle backend, std::vector<xml_value_entity_handle>& dependencies,
    std::vector<shared_ptr<attribute_entity_argument> > arguments)
{
  this->entity_type_ = entity_type;
  this->backend_ = backend;
  this->dependencies_backend_ = dependencies;
  this->arguments_ = arguments;
}

attribute_entity_argument::attribute_entity_argument(size_t index, attribute_handle& attr_obj)
{
  this->index_ = index;
  this->attribute_ = attr_obj;
}

attribute_handle attribute_entity_argument::get_attribute()
{
  return attribute_;
}

size_t attribute_entity_argument::get_index()
{
  return index_;
}

std::vector<xml_value_entity_handle> attribute_entity_cluster::get_dependencies()
{
  std::vector<xml_value_entity_handle> dependencies_concatenated = dependencies_backend_;

  for(std::vector<shared_ptr<attribute_entity_argument> >::iterator arg_iter = arguments_.begin(); arg_iter != arguments_.end(); ++arg_iter)
  {
    dependencies_concatenated.insert(
        dependencies_concatenated.end(),
        (*arg_iter)->get_attribute()->get_dependencies().begin(),
        (*arg_iter)->get_attribute()->get_dependencies().end() );
  }

  //TODO cope with double entries
  return dependencies_concatenated;
}

void attribute_entity_cluster::set_dependencies(std::vector<xml_value_entity_handle>& args)
{
  for(std::vector<xml_value_entity_handle>::iterator backend_dep_iter = dependencies_backend_.begin(); backend_dep_iter != dependencies_backend_.end(); ++backend_dep_iter)
  {
    std::string name = (*backend_dep_iter)->get_name();
    std::vector<xml_value_entity_handle>::iterator passed_arg_iter = std::find_if(args.begin(), args.end(), arg_comperator(name));

    switch((*passed_arg_iter)->get_type())
    {
      case scalar_bool:
        (*backend_dep_iter)->set_value((*passed_arg_iter)->get_value<xml_bool>());
        break;
      case scalar_int:
        (*backend_dep_iter)->set_value((*passed_arg_iter)->get_value<xml_int>());
        break;
      case scalar_float:
        (*backend_dep_iter)->set_value((*passed_arg_iter)->get_value<xml_float>());
        break;
      default:
        throw attribute_entity_error("Invalid argument type encountered");
    }
  }

  for(std::vector<shared_ptr<attribute_entity_argument> >::iterator arg_iter = arguments_.begin(); arg_iter != arguments_.end(); ++arg_iter)
  {
    if((*arg_iter)->get_attribute()->get_dependencies().empty() == false)
      (*arg_iter)->get_attribute()->set_dependencies(args);
  }
}

xml_bool attribute_entity_cluster::eval(tag_scalar_bool tag)
{
  if(entity_type_ != function_bool)
    throw func_backend_attr_type_error();

  std::vector<xml_value_entity_handle> args = evaluate_arguments();
  return backend_->eval(tag, args);
}

xml_int attribute_entity_cluster::eval(tag_scalar_int tag)
{
  if(entity_type_ != function_int)
    throw func_backend_attr_type_error();

  std::vector<xml_value_entity_handle> args = evaluate_arguments();
  return backend_->eval(tag, args);
}

xml_float attribute_entity_cluster::eval(tag_scalar_float tag)
{
  if(entity_type_ != function_float)
    throw func_backend_attr_type_error();

  std::vector<xml_value_entity_handle> args = evaluate_arguments();
  return backend_->eval(tag, args);
}

std::vector<xml_value_entity_handle> attribute_entity_cluster::evaluate_arguments()
{
  std::vector<xml_value_entity_handle> backend_args = dependencies_backend_;

  for(std::vector<shared_ptr<attribute_entity_argument> >::iterator arg_iter = arguments_.begin(); arg_iter != arguments_.end(); ++arg_iter)
  {
    if((*arg_iter)->get_attribute()->is_scalar_bool() || (*arg_iter)->get_attribute()->is_function_bool())
    {
      xml_value_entity_handle value(new xml_value_scalar_boolean);
      value->set_value((*arg_iter)->get_attribute()->evaluate<xml_bool>());
      value->set_index((*arg_iter)->get_index());
      backend_args.push_back(value);
    }else if((*arg_iter)->get_attribute()->is_scalar_int() || (*arg_iter)->get_attribute()->is_function_int())
    {
      xml_value_entity_handle value(new xml_value_scalar_integer);
      value->set_value((*arg_iter)->get_attribute()->evaluate<xml_int>());
      value->set_index((*arg_iter)->get_index());
      backend_args.push_back(value);
    }else if((*arg_iter)->get_attribute()->is_scalar_float() || (*arg_iter)->get_attribute()->is_function_float())
    {
      xml_value_entity_handle value(new xml_value_scalar_float);
      value->set_value((*arg_iter)->get_attribute()->evaluate<xml_float>());
      value->set_index((*arg_iter)->get_index());
      backend_args.push_back(value);
    }else if((*arg_iter)->get_attribute()->is_tensor() || (*arg_iter)->get_attribute()->is_function_tensor())
    {
      //TODO tensor
      throw attribute_entity_error("Tensor not yet implemented");
    }
  }
  return backend_args;
}

} /* namespace viennamaterials */
