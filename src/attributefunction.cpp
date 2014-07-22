/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/attributefunction.h"
#include "viennamaterials/utils/convert.hpp"

namespace viennamaterials
{

long IAttributeFunction::query_number_of_attributes(library_handle& lib, std::string& xpath_query)
{
  std::string query = "count(" + xpath_query + "/@*)";
  return lib->query_xpath_number(query);
}

std::string IAttributeFunction::query_attribute_name(library_handle& lib, std::string& xpath_query, long position)
{
  //TODO: check validity position?
  std::ostringstream position_string;
  position_string << position;
  std::string query = "name(" + xpath_query + "/@*[" + position_string.str() + "])";
  return lib->query_xpath_string(query);
}

std::string IAttributeFunction::query_attribute(library_handle& lib, std::string& xpath_query, long position)
{
  //TODO: check validity position?
  std::ostringstream position_string;
  position_string << position;
  std::string query = "string(" + xpath_query + "/@*[" + position_string.str() + "])";
  return lib->query_xpath_string(query);
}

std::string IAttributeFunction::query_attribute(library_handle& lib, std::string& xpath_query, std::string attribute_name)
{
  std::string query = "string(" + xpath_query + "/@" + attribute_name + ")";
  return lib->query_xpath_string(query);
}

long IAttributeFunction::query_number_of_arguments(library_handle& lib, std::string& xpath_query_to_function)
{
  std::string query = "count(" + xpath_query_to_function + "/arg)";
  return lib->query_xpath_number(query);
}

XmlType IAttributeFunction::query_return_type(library_handle& lib, std::string& xpath_query_to_function)
{
  std::string query = "name(" + xpath_query_to_function + "/return/*)";
  std::string entity_string = lib->query_xpath_string(query);
  if(entity_string.compare("scalar") == 0)
  {
    query = "string(" + xpath_query_to_function + "/return/scalar/@type)";
    std::string type_string = lib->query_xpath_string(query);
    if(type_string.compare("bool") == 0)
      return scalar_bool;
    else if(type_string.compare("int") == 0)
      return scalar_int;
    else if(type_string.compare("float") == 0)
      return scalar_float;
  }
  else if(entity_string.compare("tensor") == 0)
    return tensor;
  else if(entity_string.compare("function") == 0)
    return tensor; //FIXME

  return scalar_bool; //TODO error handling? (exception or 'invalid type')
}

std::vector<FunctionArgumentBase>  IAttributeFunction::query_argument(viennamaterials::library_handle& lib, std::string& xpath_query_to_function, long xml_index)
{
  //TODO: implemented function and tensor given as arg

  //TODO id of arg

  std::vector<FunctionArgumentBase> argument_vector;

  std::ostringstream xml_index_string;
  xml_index_string << xml_index;
  std::string query_arg = xpath_query_to_function + "/arg[" + xml_index_string.str() + "]";
  std::string query = "name(" + query_arg + "/scalar|" + query_arg + "/tensor|" + query_arg + "/function|" + query_arg + "/reference" + ")";
  std::string entity_string = lib->query_xpath_string(query);

  query = query_arg + "/quantity/text()";
  std::string arg_quantity = lib->query(query);

  if(entity_string.compare("scalar") == 0)
  {
    query = "string(" + query_arg + "/scalar/@type)";
    std::string type_string = lib->query_xpath_string(query);
    if(type_string.compare("bool") == 0)
    {
      FunctionArgument<xml_bool> argument;
      argument.type = scalar_bool;
      argument.quantity = arg_quantity;
      argument.value = false; //FIXME TODO define representation of bool value in XML (ture, false, 1, 0, ???)
      argument_vector.insert(argument_vector.begin(), argument);
      return argument_vector;
    }
    else if(type_string.compare("int") == 0)
    {
      FunctionArgument<xml_int> argument;
      argument.type = scalar_int;
      argument.quantity = arg_quantity;
      query = query_arg + "/scalar/text()";
      argument.value = convert<xml_int>(lib->query(query));
      argument_vector.insert(argument_vector.begin(), argument);
      return argument_vector;
    }
    else if(type_string.compare("float") == 0)
    {
      FunctionArgument<xml_float> argument;
      argument.type = scalar_float;
      argument.quantity = arg_quantity;
      query = query_arg + "/scalar/text()";
      argument.value = convert<xml_float>(lib->query(query));
      argument_vector.insert(argument_vector.begin(), argument);
      return argument_vector;
    }
  }

  return argument_vector;
}

std::vector<FunctionArgument<xml_float> >  IAttributeFunction::query_argument_testing(viennamaterials::library_handle& lib, std::string& xpath_query_to_function, long xml_index)
{
  //TODO: implemented function and tensor given as arg

  //TODO id of arg

  std::vector<FunctionArgument<xml_float> > argument_vector;

  std::ostringstream xml_index_string;
  xml_index_string << xml_index;
  std::string query_arg = xpath_query_to_function + "/arg[" + xml_index_string.str() + "]";
  std::string query = "name(" + query_arg + "/scalar|" + query_arg + "/tensor|" + query_arg + "/function|" + query_arg + "/reference" + ")";
  std::string entity_string = lib->query_xpath_string(query);

  query = query_arg + "/quantity/text()";
  std::string arg_quantity = lib->query(query);

  if(entity_string.compare("scalar") == 0)
  {
    query = "string(" + query_arg + "/scalar/@type)";
    std::string type_string = lib->query_xpath_string(query);
    if(type_string.compare("float") == 0)
    {
      FunctionArgument<xml_float> argument;
      argument.type = scalar_float;
      argument.quantity = arg_quantity;
      query = query_arg + "/scalar/text()";
      argument.value = convert<xml_float>(lib->query(query));
      argument_vector.insert(argument_vector.begin(), argument);
      return argument_vector;
    }
  }

  return argument_vector;
}

std::vector<FunctionArgumentBase> IAttributeFunction::query_arguments(viennamaterials::library_handle& lib, std::string& xpath_query_to_function)
{
  /*
   * return
   * number of args
   * args
   */

//  XmlType return_type = query_return_type(lib, xpath_query_to_function);
//  std::vector<FunctionArgumentBase> arguments;
//  if(return_type == scalar_bool)
//  {
//    FunctionArgument<bool> return_arg;
//    return_arg.quantity = "result";
//    return_arg.type = scalar_bool;
//    return_arg.value = false;
//    arguments.insert(arguments.begin(), return_arg);
//  }

  std::vector<FunctionArgumentBase> arguments;

  long number_of_args = query_number_of_arguments(lib, xpath_query_to_function);
  if(number_of_args == 0)
    return arguments;

  long i;
  for(i = 1; i <= number_of_args; i++)
  {
    //TODO
  }

  return arguments;
}

std::vector<FunctionArgument<xml_float> > IAttributeFunction::query_arguments_testing(viennamaterials::library_handle& lib, std::string& xpath_query_to_function)
{
  std::vector<FunctionArgument<xml_float> > arguments;

  long number_of_args = query_number_of_arguments(lib, xpath_query_to_function);
  if(number_of_args == 0)
    return arguments;

  long i;
  for(i = 1; i <= number_of_args; i++)
  {
    std::vector<FunctionArgument<xml_float> > argument;
    argument = query_argument_testing(lib, xpath_query_to_function, i);
    arguments.insert(arguments.end(), argument.front());
  }

  return arguments;
}

} /* namespace viennamaterials */
