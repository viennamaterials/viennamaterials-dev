/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "../viennamaterials/broker.hpp"
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/exceptions.hpp"
#include "viennamaterials/xmlvaluescalar.hpp"
#include "viennamaterials/utils/convert.hpp"
#include "viennamaterials/functionbackendpython.hpp"
#include "viennamaterials/attributeentityfunction.hpp"
#include <vector>

namespace viennamaterials
{

broker::broker(std::string const& filename)
{
  /// Create a material library object
  lib_ = viennamaterials::generator(filename);
}

attribute_handle broker::query(std::string const& xpath_query_to_attribute)
{
  /// Gather data from XML
  xml_attribute_type type = get_attribute_type(xpath_query_to_attribute);

  if(type == function_bool || type == function_int || type == function_float) //TODO function_tensor
  {
    std::string query = xpath_query_to_attribute + "/function";
    std::string code_language = lib_->query_attribute(query + "/code", "lang");

    /// Load function arguments
    long number_of_arguments = lib_->query_number_of_elements(query + "/arg");
    std::vector<xml_value_entity*> args;

    /// Note: is skipped if number_of_arguments == 0
    /// Note: XML indexing begins with 1
    for(long i = 1; i <= number_of_arguments; i++)
    {
      /// Handle each argument

      //TODO cope with reference arg

      xml_value_entity *entity_ptr = 0;
      std::ostringstream index_string;
      index_string << i;
      std::string query_arg = query + "/arg[" + index_string.str() + "]";

      if(lib_->query_number_of_elements(query_arg + "/scalar") == 1)
      {
        std::string query_arg_scalar = query_arg + "/scalar";
        std::string type_attribute = "type";
        if(lib_->query_attribute(query_arg_scalar, type_attribute).compare("bool") == 0)
        {
          //TODO scalar bool
          throw broker_error("Bool scalar not yet implemented");
        }else if(lib_->query_attribute(query_arg_scalar, type_attribute).compare("int") == 0)
        {
          entity_ptr = new xml_value_scalar_integer;
          entity_ptr->set_value( convert<xml_int>(lib_->query(query_arg_scalar + "/text()")) );
        }else if(lib_->query_attribute(query_arg_scalar, type_attribute).compare("float") == 0)
        {
          entity_ptr = new xml_value_scalar_float;
          entity_ptr->set_value( convert<xml_float>(lib_->query(query_arg_scalar + "/text()")) );
        }


      }else if(lib_->query_number_of_elements(query_arg + "/tensor") == 1)
      {
        //TODO tensor arg
        throw broker_error("Tensor not yet implemented");
      }else
        throw broker_error("Invalid function argument type encountered (query: " + query_arg + ")");

      entity_ptr->set_index( convert<size_t>(lib_->query(query_arg + "/id/text()")) );
      entity_ptr->set_name(lib_->query(query_arg + "/quantity/text()"));

      args.push_back(entity_ptr);
    }


    /// Load function backend
    function_backend_handle backend;
    std::string query_code = query + "/code";
    std::string lang_attribute = "lang";
    std::string call_attribute = "call";
    if(lib_->query_attribute(query_code, lang_attribute).compare("python") == 0)
    {
      function_backend_handle tmp(new function_backend_python);
      backend = tmp;
    }
    else
      throw broker_error("Not supported code language encountered (query: " + query_code + ")");

    backend->init(lib_->query(query_code + "/text()"), lib_->query_attribute(query_code, call_attribute));


    /// Create function attribute entity
    attribute_handle entity(new attribute_entity_function(type, backend, args));
    return entity;
  }

  //TODO tensor
  //TODO scalar

  throw broker_error("Invalid argument type encountered (query: " + xpath_query_to_attribute + ")");
}

xml_attribute_type broker::get_attribute_type(std::string const& xpath_query_to_attribute)
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
    throw broker_error("No valid return type for function attribute found (query: " + query + ")");

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

    throw broker_error("No valid type for scalar attribute found (query: " + query + ")");
  }

  throw broker_error("No valid attribute type found (query: " + xpath_query_to_attribute + ")");
}

} /* namespace viennamaterials */
