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

xml_attribute_entity IAttributeFunction::query_return_type(library_handle& lib, std::string& xpath_query_to_function)
{
  //FIXME: return (entity + type)

  std::string query = "name(" + xpath_query_to_function + "/return/*)";
  std::string type_string = lib->query_xpath_string(query);
  if(type_string.compare("scalar") == 0)
    return scalar;
  else if(type_string.compare("tensor") == 0)
    return tensor;
  else if(type_string.compare("function") == 0)
    return function;

  return scalar; //TODO error handling? (exception or 'invalid type')
}

} /* namespace viennamaterials */
