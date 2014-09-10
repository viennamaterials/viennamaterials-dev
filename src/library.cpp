/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/library.hpp"
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/exceptions.hpp"
#include "viennamaterials/xmlvaluescalar.hpp"
#include "viennamaterials/utils/convert.hpp"
#include "viennamaterials/functionbackendpython.hpp"
#include "viennamaterials/attributeentityscalar.hpp"
#include "viennamaterials/attributeentityfunction.hpp"
#include <vector>

namespace viennamaterials
{

library::library(std::string const& filename)
{
  /// Create a material library object
  lib_ = viennamaterials::generator(filename);
}

attribute_handle library::query(std::string const& xpath_query_to_attribute)
{
  /// Gather data from XML
  xml_attribute_type type = get_attribute_type(xpath_query_to_attribute);

  /// Query unit
  std::string unit = lib_->query(xpath_query_to_attribute + "/unit/text()");

  if(type == function_bool || type == function_int || type == function_float) //TODO tensor: function_tensor
  {
    std::string query = xpath_query_to_attribute + "/function";
    std::string code_language = lib_->query_attribute(query + "/code", "lang");

    /// Load function arguments
    long number_of_arguments = lib_->query_number_of_elements(query + "/arg");
    std::vector<xml_value_entity_handle> dependencies;
    std::vector<shared_ptr<attribute_entity_argument> > referenced_arguments;

    /// Note: is skipped if number_of_arguments == 0
    /// Note: XML indexing begins with 1
    for(long i = 1; i <= number_of_arguments; i++)
    {
      /// Handle each argument

      xml_value_entity_handle entity_ptr;
      std::string query_arg = query + "/arg[" + convert<std::string>(i) + "]";

      if(lib_->query_number_of_elements(query_arg + "/scalar") == 1)
      {
        std::string query_arg_scalar = query_arg + "/scalar";
        const std::string type_attribute = "type";
        if(lib_->query_attribute(query_arg_scalar, type_attribute).compare("bool") == 0)
        {
          xml_value_entity_handle bool_entity(new xml_value_scalar_boolean);
          entity_ptr = bool_entity;
          std::string value = lib_->query(query_arg_scalar + "/text()");
          std::transform(value.begin(), value.end(), value.begin(), ::tolower);
          if(value.compare("true") == 0)
            entity_ptr->set_value(true);
          else if(value.compare("false") == 0)
            entity_ptr->set_value(false);
          else
            throw library_error("Invalid boolean value encountered (query: " + query_arg_scalar + ")");
        }else if(lib_->query_attribute(query_arg_scalar, type_attribute).compare("int") == 0)
        {
          xml_value_entity_handle int_entity(new xml_value_scalar_integer);
          entity_ptr = int_entity;
          entity_ptr->set_value( convert<xml_int>(lib_->query(query_arg_scalar + "/text()")) );
        }else if(lib_->query_attribute(query_arg_scalar, type_attribute).compare("float") == 0)
        {
          xml_value_entity_handle float_entity(new xml_value_scalar_float);
          entity_ptr = float_entity;
          entity_ptr->set_value( convert<xml_float>(lib_->query(query_arg_scalar + "/text()")) );
        }


      }else if(lib_->query_number_of_elements(query_arg + "/tensor") == 1)
      {
        //TODO tensor arg
        throw library_error("Tensor not yet implemented");
      }else if(lib_->query_number_of_elements(query_arg + "/reference") == 1)
      {
        std::string query_referenced_attribute = lib_->query(query_arg + "/reference/text()");
        attribute_handle referenced_attribute = this->query(query_referenced_attribute); //broker query
        shared_ptr<attribute_entity_argument> attribute_arg_ptr(
            new attribute_entity_argument(convert<size_t>(lib_->query(query_arg + "/id/text()")), referenced_attribute));
        referenced_arguments.push_back(attribute_arg_ptr);
        continue;
      }else
        throw library_error("Invalid function argument type encountered (query: " + query_arg + ")");

      entity_ptr->set_index( convert<size_t>(lib_->query(query_arg + "/id/text()")) );
      entity_ptr->set_name(lib_->query(query_arg + "/quantity/text()"));

      dependencies.push_back(entity_ptr);
    }


    /// Load function backend
    function_backend_handle backend;
    std::string query_code = query + "/code";
    const std::string lang_attribute = "lang";
    const std::string call_attribute = "call";
    if(lib_->query_attribute(query_code, lang_attribute).compare("python") == 0)
    {
      function_backend_handle tmp(new function_backend_python);
      backend = tmp;
    }
    else
      throw library_error("Not supported code language encountered (query: " + query_code + ")");

    backend->init(lib_->query(query_code + "/text()"), lib_->query_attribute(query_code, call_attribute));

    ///load unit

    /// Create function attribute entity
    attribute_handle entity(new attribute_entity_function(unit, type, backend, dependencies, referenced_arguments));

    return entity;
  }else if(type == scalar_bool || type == scalar_int || type ==  scalar_float)
  {
    std::string query_scalar = xpath_query_to_attribute + "/scalar";
    const std::string type_attribute = "type";
    if(lib_->query_attribute(query_scalar, type_attribute).compare("bool") == 0)
    {
      std::string value = lib_->query(query_scalar + "/text()");
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      if(value.compare("true") == 0)
      {
        /// Create boolean scalar attribute entity holding value true
        attribute_handle entity(new attribute_entity_scalar_boolean(true, unit));
        return entity;
      }
      else if(value.compare("false") == 0)
      {
        /// Create boolean scalar attribute entity holding value false
        attribute_handle entity(new attribute_entity_scalar_boolean(false, unit));
        return entity;
      }
      else
        throw library_error("Invalid boolean value encountered (query: " + query_scalar + ")");
    }else if(lib_->query_attribute(query_scalar, type_attribute).compare("int") == 0)
    {
      /// Create integer scalar attribute entity
      xml_int value = convert<xml_int>(lib_->query(query_scalar + "/text()"));
      attribute_handle entity(new attribute_entity_scalar_integer(value, unit));
      return entity;
    }else if(lib_->query_attribute(query_scalar, type_attribute).compare("float") == 0)
    {
      /// Create floating point scalar attribute entity
      xml_float value = convert<xml_float>(lib_->query(query_scalar + "/text()"));
      attribute_handle entity(new attribute_entity_scalar_float(value, unit));
      return entity;
    }
  }

  //TODO tensor

  throw library_error("Invalid argument type encountered (query: " + xpath_query_to_attribute + ")");
}

xml_attribute_type library::get_attribute_type(std::string const& xpath_query_to_attribute)
{
  /**
   * Precedence of attribute types:
   *  Function is prefered over scalar or tensor.
   *  Tensor is prefered over scalar.
   *  First function element is prefered if two or more functions are defined.
   */

  std::string type_attribute = "type";

  if(lib_->query_number_of_elements(xpath_query_to_attribute + "/function") > 0)
  {
    std::string query = xpath_query_to_attribute + "/function/return";

    if(lib_->query_number_of_elements(query + "/tensor") == 1)
      return function_tensor;
    else if(lib_->query_number_of_elements(query + "/scalar") == 1)
    {
      query = query + "/scalar";

      if( lib_->query_attribute(query, type_attribute).compare("bool") == 0 )
        return function_bool;
      else if( lib_->query_attribute(query, type_attribute).compare("int") == 0 )
        return function_int;
      else if( lib_->query_attribute(query, type_attribute).compare("float") == 0 )
        return function_float;
    }
    throw library_error("No valid return type for function attribute found (query: " + query + ")");

  }else if(lib_->query_number_of_elements(xpath_query_to_attribute + "/tensor") > 0)
  {
    return tensor;
  }
  else if(lib_->query_number_of_elements(xpath_query_to_attribute + "/scalar") > 0)
  {
    std::string query = xpath_query_to_attribute + "/scalar";

    if( lib_->query_attribute(query, type_attribute).compare("bool") == 0 )
      return scalar_bool;
    else if( lib_->query_attribute(query, type_attribute).compare("int") == 0 )
      return scalar_int;
    else if( lib_->query_attribute(query, type_attribute).compare("float") == 0 )
      return scalar_float;

    throw library_error("No valid type for scalar attribute found (query: " + query + ")");
  }

  throw library_error("No valid attribute type found (query: " + xpath_query_to_attribute + ")");
}

} /* namespace viennamaterials */
