/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/utils.hpp"

#include "boost/algorithm/string/trim.hpp"

namespace viennamaterials {

pugixml::pugixml() : indent_string_("  ")
{
  this->init();
}

pugixml::pugixml(std::string const& filename) : indent_string_("  ")
{
  this->init();
  this->read(filename);
}

pugixml::~pugixml()
{
  delete query_material_;
  delete query_category_;
  delete query_parameter_;
//  delete query_parameter_value_;
  delete query_parameter_unit_;
  delete query_parameter_note_;
}

void pugixml::init()
{
  // setup variables used as placeholdes in the precompiled queries
  vars_.add(viennamaterials::key::id.c_str(),         pugi::xpath_type_string);
  vars_.add(viennamaterials::key::category.c_str(),   pugi::xpath_type_string);
  vars_.add(viennamaterials::key::parameter.c_str(),  pugi::xpath_type_string);

  // precompile the various query strings - use placeholders for run-time parameters
  std::string material_query_string = "/materials/material[id = string($"+viennamaterials::key::id+")]";
  query_material_ = new pugi::xpath_query(material_query_string.c_str(), &vars_);
  
  std::string category_query_string = "/materials/material[category = string($"+viennamaterials::key::category+")]";
  query_category_ = new pugi::xpath_query(category_query_string.c_str(), &vars_);

  std::string parameter_query_string = "/materials/material[id = string($"+viennamaterials::key::id+")]/parameters/parameter[name = string($"+viennamaterials::key::parameter+")]";
  query_parameter_ = new pugi::xpath_query(parameter_query_string.c_str(), &vars_);    

//  std::string parameter_value_query_string = "/materials/material[id = string($"+viennamaterials::key::id+")]/parameters/parameter[name = string($"+viennamaterials::key::parameter+")]/value";
//  query_parameter_value_ = new pugi::xpath_query(parameter_value_query_string.c_str(), &vars_);    

  std::string parameter_unit_query_string = "/materials/material[id = string($"+viennamaterials::key::id+")]/parameters/parameter[name = string($"+viennamaterials::key::parameter+")]/unit";
  query_parameter_unit_ = new pugi::xpath_query(parameter_unit_query_string.c_str(), &vars_);    
  
  std::string parameter_note_query_string = "/materials/material[id = string($"+viennamaterials::key::id+")]/parameters/parameter[name = string($"+viennamaterials::key::parameter+")]/note";
  query_parameter_note_ = new pugi::xpath_query(parameter_note_query_string.c_str(), &vars_);      
}

bool pugixml::read(std::string const& filename)
{
  if(viennamaterials::file_extension(filename) != "xml")  
  {
    throw no_xml_file_error();
    return false;
  }

  xml_.reset();
#ifdef VIENNAMATERIALS_HAS_LIBXML2
  if(!viennamaterials::check(filename))
  {
    return false;
  }
#endif

  if(!viennamaterials::file_exists(filename))
  { 
    throw xml_file_load_error();
    return false;
  }

  pugi::xml_parse_result result = xml_.load_file(filename.c_str());
  if(!result)
    throw viennamaterials::xml_parse_error(std::string(result.description()));

  return true;
}

bool pugixml::read(std::stringstream & stream)
{
  xml_.reset();

  pugi::xml_parse_result result = xml_.load(stream);
  if(!result)
    throw viennamaterials::xml_parse_error(std::string(result.description()));

  return true;
}

bool pugixml::write(std::stringstream& stream)
{
  dump(stream);
  return true;
}

bool pugixml::write(std::ofstream& ostream)
{
  dump(ostream);
  return true;
}

bool pugixml::write(std::string const& filename)
{
  std::ofstream stream(filename.c_str());
  dump(stream);
  stream.close();
  return true;
}

void pugixml::dump(std::ostream& stream)
{
  xml_.save(stream, indent_string_.c_str());
}

bool pugixml::has_parameter(std::string const& material, std::string const& parameter)
{
  pugi::xpath_node_set const& result = this->query_parameter(material, parameter);
  if(result.size() > 1) // there must be only one material with this id
  {
    throw viennamaterials::non_unique_parameter_exception(parameter);
    return false;
  }
  else if (result.size() == 0) return false;
  else return true;
}

viennamaterials::numeric pugixml::get_parameter_value(std::string const& material, std::string const& parameter)
{
  // [JW] this method works, as the built-in 'evaluate_number' method can not 
  // handle scientific E notations
  std::string value_string = this->query("/materials/material[id = \""+material+"\"]/parameters/parameter[name = \""+parameter+"\"]/value/text()");
  viennamaterials::numeric result;
  std::stringstream sstr;
  sstr << value_string;
  sstr >> result;
  return result;
}

std::string pugixml::query(std::string const& xpath_query_str)
{
  std::stringstream result_stream;
  try
  {
     pugi::xpath_node_set query_result = xml_.select_nodes(xpath_query_str.c_str());
     for(size_t i = 0; i < query_result.size(); i++)
     {
       query_result[i].node().print(result_stream, "  ");
     }
  }
  catch (const pugi::xpath_exception& e)
  {
     std::cerr << "Exception caught in XmlReader::query ->  " << e.what() << std::endl;
  }
  std::string result = result_stream.str();
  boost::trim(result);  // remove front/trailing whitespaces
  return result;
}

std::string pugixml::get_parameter_unit(std::string const& material, std::string const& parameter)
{
  vars_.set(viennamaterials::key::id.c_str(),        material.c_str());  
  vars_.set(viennamaterials::key::parameter.c_str(), parameter.c_str());
  return query_parameter_unit_->evaluate_string(xml_);
}

viennamaterials::keys pugixml::get_materials_of_category(std::string const& category)
{
  vars_.set(viennamaterials::key::category.c_str(), category.c_str());
  pugixml::node_set_type entries =  query_category_->evaluate_node_set(xml_);
  viennamaterials::keys keys;
  for(pugixml::node_iterator_type iter = entries.begin(); iter != entries.end(); iter++)
    keys.push_back( this->id(*iter) );
  return keys;
}

bool pugixml::has_materials_of_category(std::string const& category)
{
  return !(this->get_materials_of_category(category).empty());
}

pugixml::node_set_type pugixml::query_parameter(std::string const& material, std::string const& parameter)
{
  vars_.set(viennamaterials::key::id.c_str(),        material.c_str());  
  vars_.set(viennamaterials::key::parameter.c_str(), parameter.c_str());
  return query_parameter_->evaluate_node_set(xml_);
}

std::string pugixml::id(pugixml::node_type const& entry)
{
  return pugi::xpath_query("id").evaluate_string(entry);
}


} // viennamaterials

