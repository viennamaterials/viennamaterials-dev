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

pugixml::pugixml() 
{
  this->init();
}

pugixml::pugixml(std::string const& filename)
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
}

void pugixml::dump(std::ostream& stream)
{
  database_.dump(stream);
}

bool pugixml::has_parameter(std::string const& material, std::string const& parameter)
{
//  pugi::xpath_node_set const& result = this->query_parameter(material_id, parameter_id);
//  if(result.size() > 1) // there must be only one material with this id
//  {
//    throw viennamaterials::non_unique_parameter_exception(parameter);
//    return false;
//  }
//  else if (result.size() == 0) return false;
//  else return true;
}

pugixml::numeric_type pugixml::get_parameter_value(std::string const& material, std::string const& parameter)
{
}

std::string pugixml::get_parameter_unit(std::string const& material, std::string const& parameter)
{
}

pugixml::keys_type pugixml::get_materials_of_category(std::string const& category)
{
}

bool pugixml::has_materials_of_category(std::string const& category)
{
}

//pugixml::EntriesType pugixml::query_parameter(pugixml::EntryType const& material, std::string const& parameter)
//{
////  vars_.set(viennamaterials::key::parameter.c_str(), parameter_id.c_str());
////  return query_parameter->evaluate_node_set(material.node());
//}

} // viennamaterials

