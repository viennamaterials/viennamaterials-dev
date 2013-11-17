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


namespace viennamaterials {

pugixml::pugixml() : indent_string("  ")
{
  this->init();
}

pugixml::pugixml(std::string const& filename) : indent_string("  ")
{
  this->init();
  this->load(filename);
}

pugixml::~pugixml()
{
  delete query_material_;
  delete query_category_;
  delete query_parameter_;
  delete query_parameter_value_;
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

  std::string parameter_value_query_string = "/materials/material[id = string($"+viennamaterials::key::id+")]/parameters/parameter[name = string($"+viennamaterials::key::parameter+")]/value";
  query_parameter_value_ = new pugi::xpath_query(parameter_value_query_string.c_str(), &vars_);    

  std::string parameter_unit_query_string = "/materials/material[id = string($"+viennamaterials::key::id+")]/parameters/parameter[name = string($"+viennamaterials::key::parameter+")]/unit";
  query_parameter_unit_ = new pugi::xpath_query(parameter_unit_query_string.c_str(), &vars_);    
  
  std::string parameter_note_query_string = "/materials/material[id = string($"+viennamaterials::key::id+")]/parameters/parameter[name = string($"+viennamaterials::key::parameter+")]/note";
  query_parameter_note_ = new pugi::xpath_query(parameter_note_query_string.c_str(), &vars_);      
}

bool pugixml::load(std::string const& filename)
{
  database_.reset();
#ifdef HAVE_LIBXML2
  if(!viennamaterials::check(filename))
  {
    return false;
  }
#endif

  if(!viennautils::file_exists(filename))
  { 
    throw xml_file_load_error();
    return false;
  }

  if(viennautils::file_extension(filename) == "xml")  // native
  {
    database_.read(filename);
    return true;
  }
  else return false;
}

bool pugixml::load(std::stringstream & stream)
{
  database_.reset();
  database_.read(stream);
  return true;
}

void pugixml::dump(std::ostream& stream)
{
  database_.dump(stream);
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
  vars_.set(viennamaterials::key::id.c_str(),        material.c_str());  
  vars_.set(viennamaterials::key::parameter.c_str(), parameter.c_str());
  return query_parameter_value_->evaluate_number(database_.xml);
}

std::string pugixml::get_parameter_unit(std::string const& material, std::string const& parameter)
{
  vars_.set(viennamaterials::key::id.c_str(),        material.c_str());  
  vars_.set(viennamaterials::key::parameter.c_str(), parameter.c_str());
  return query_parameter_unit_->evaluate_string(database_.xml);
}

viennamaterials::keys pugixml::get_materials_of_category(std::string const& category)
{
  vars_.set(viennamaterials::key::category.c_str(), category.c_str());
  pugixml::EntriesType entries =  query_category_->evaluate_node_set(database_.xml);
  viennamaterials::keys keys;
  for(pugixml::EntryIteratorType iter = entries.begin(); iter != entries.end(); iter++)
    keys.push_back( this->id(*iter) );
  return keys;
}

bool pugixml::has_materials_of_category(std::string const& category)
{
  return !(this->get_materials_of_category(category).empty());
}

pugixml::EntriesType pugixml::query_parameter(std::string const& material, std::string const& parameter)
{
  vars_.set(viennamaterials::key::id.c_str(),        material.c_str());  
  vars_.set(viennamaterials::key::parameter.c_str(), parameter.c_str());
  return query_parameter_->evaluate_node_set(database_.xml);
}

std::string pugixml::id(pugixml::EntryType const& entry)
{
  return pugi::xpath_query("id").evaluate_string(entry);
}

} // viennamaterials

