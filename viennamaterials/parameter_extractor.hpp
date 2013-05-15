#ifndef VIENNAMATERIALS_PARAMETEREXTRACTOR_HPP
#define VIENNAMATERIALS_PARAMETEREXTRACTOR_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "forwards.h"

namespace vmat
{
struct ParameterExtractor
{
public:
  ParameterExtractor()
  {
    vars.add(vmat::key::parameter.c_str(), pugi::xpath_type_string);
    std::string parameter_query_string = ".//parameters/parameter[name = string($"+vmat::key::parameter+")]";
    query_parameter = new pugi::xpath_query(parameter_query_string.c_str(), &vars);    
  }

  ~ParameterExtractor()
  {
    delete query_parameter;
  }

  Entry operator()(Entry const& material, std::string const& parameter_id)
  {
    pugi::xpath_node_set const& result = this->queryParameter(material, parameter_id);
    if(result.size() > 1) // there must be only one parameter with this id
    {
      throw vmat::NonUniqueParameterException(parameter_id);
      return Entry();
    }
    else if (result.size() == 0) return Entry();
    else return result.first(); // if success, return only a single xpath_node
  }

  bool hasParameter(Entry const& material, std::string const& parameter_id)
  {
    pugi::xpath_node_set const& result = this->queryParameter(material, parameter_id);
    if(result.size() > 1) // there must be only one material with this id
    {
      throw vmat::NonUniqueParameterException(parameter_id);
      return false;
    }
    else if (result.size() == 0) return false;
    else return true;
  }  

private:
  Entries queryParameter(Entry const& material, std::string const& parameter_id)
  {
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter->evaluate_node_set(material.node());
  }

  pugi::xpath_variable_set    vars;
  
  pugi::xpath_query *query_parameter;  
};

Entry getParameter(Entry const& material, std::string const& parameter_id)
{
  return vmat::ParameterExtractor()(material, parameter_id);
}

bool hasParameter(Entry const& material, std::string const& parameter_id)
{
  return vmat::ParameterExtractor().hasParameter(material, parameter_id);
}


} // end namespace vmat

#endif
