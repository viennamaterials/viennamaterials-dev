#ifndef VIENNAMATERIALS_LIBRARY_HPP
#define VIENNAMATERIALS_LIBRARY_HPP


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
#include "io.hpp"
#include "exceptions.hpp"
#include "parameter_extractor.hpp"
#include "tools.hpp"
#include "check.hpp"

namespace vmat { 



struct Library 
{
  Library() 
  {
    // setup variables used as placeholdes in the precompiled queries
    vars.add(vmat::key::id.c_str(),         pugi::xpath_type_string);
    vars.add(vmat::key::category.c_str(),   pugi::xpath_type_string);
    vars.add(vmat::key::parameter.c_str(),  pugi::xpath_type_string);

    // precompile the various query strings - use placeholders for run-time parameters
    std::string material_query_string = "/materials/material[id = string($"+vmat::key::id+")]";
    query_material = new pugi::xpath_query(material_query_string.c_str(), &vars);
    
    std::string category_query_string = "/materials/material[category = string($"+vmat::key::category+")]";
    query_category = new pugi::xpath_query(category_query_string.c_str(), &vars);

    std::string parameter_query_string = "/materials/material[id = string($"+vmat::key::id+")]/parameters/parameter[name = string($"+vmat::key::parameter+")]";
    query_parameter = new pugi::xpath_query(parameter_query_string.c_str(), &vars);    

    std::string parameter_value_query_string = "/materials/material[id = string($"+vmat::key::id+")]/parameters/parameter[name = string($"+vmat::key::parameter+")]/value";
    query_parameter_value = new pugi::xpath_query(parameter_value_query_string.c_str(), &vars);    

    std::string parameter_unit_query_string = "/materials/material[id = string($"+vmat::key::id+")]/parameters/parameter[name = string($"+vmat::key::parameter+")]/unit";
    query_parameter_unit = new pugi::xpath_query(parameter_unit_query_string.c_str(), &vars);    
    
    std::string parameter_note_query_string = "/materials/material[id = string($"+vmat::key::id+")]/parameters/parameter[name = string($"+vmat::key::parameter+")]/note";
    query_parameter_note = new pugi::xpath_query(parameter_note_query_string.c_str(), &vars);    
  }
  
  ~Library()
  {
    // the xpath queries are due to pugixml's impelmentation required to be allocated
    // on the heap ..
    delete query_material;
    delete query_category;
    delete query_parameter;
    delete query_parameter_value;
    delete query_parameter_unit;
    delete query_parameter_note;
  }

  bool load(std::string const& filename)
  {
#ifdef HAVE_LIBXML2
    if(!vmat::check(filename))
    {
      return false;
    }
#endif
  
    if(!viennautils::file_exists(filename)) return false;
    
    if(viennautils::file_extension(filename) == "xml")  // native
    {
      mdb.read(filename);
      return true;
    }
#ifdef HAVE_VIENNAIPD    
    else if(viennautils::file_extension(filename) == "ipd")  // conversion required
    {
      ipdInit(NULL, NULL);
      
      // todo: 
      std::cout << "IPD support is not yet implemented .." << std::endl;
      
      return true;
    }
#endif
    else return false;
  }
  
  
  void dump(std::ostream& stream = std::cout)
  {
    mdb.dump(stream);
  }

  Entries query(std::string const& expr)
  {
    return mdb.query_raw(expr);
  }

  Entries query(std::string const& expr) const
  {
    return mdb.query_raw(expr);
  }

  Entries getSemiconductors()
  {
    vars.set(vmat::key::category.c_str(), vmat::key::semiconductor.c_str());
    return query_category->evaluate_node_set(mdb.xml);
  }
  
  bool hasSemiconductors()
  {
    return !(this->getSemiconductors().empty());
  }  

  Entries getMetals()
  {
    vars.set(vmat::key::category.c_str(), vmat::key::metal.c_str());
    return query_category->evaluate_node_set(mdb.xml);
  }
  
  bool hasMetals()
  {
    return !(this->getMetals().empty());
  }  

  Entries getOxides()
  {
    vars.set(vmat::key::category.c_str(), vmat::key::oxide.c_str());
    return query_category->evaluate_node_set(mdb.xml);
  }
  
  bool hasOxides()
  {
    return !(this->getOxides().empty());
  }  

  Entries getMaterialsOfCategory(std::string const& category_id)
  {
    vars.set(vmat::key::category.c_str(), category_id.c_str());
    return query_category->evaluate_node_set(mdb.xml);
  }
  
  bool hasMaterialsOfCategory(std::string const& category_id)
  {
    return !(this->getMaterialsOfCategory(category_id).empty());
  }  

  // -- Material Accessors -----------------------------------------------------
private:  
  Entries queryMaterial(std::string const& material_id)
  {
    vars.set(vmat::key::id.c_str(), material_id.c_str());
    return query_material->evaluate_node_set(mdb.xml);
  }
public:
  Entry getMaterial(std::string const& material_id)
  {
    pugi::xpath_node_set const& result = this->queryMaterial(material_id);
    if(result.size() > 1) // there must be only one material with this id
    {
      throw vmat::NonUniqueMaterialException(material_id);
      return Entry();
    }
    else if (result.size() == 0) return Entry();
    else return result.first(); // if success, return only a single xpath_node
  }
  
  bool hasMaterial(std::string const& material_id)
  {
    pugi::xpath_node_set const& result = this->queryMaterial(material_id);
    if(result.size() > 1) // there must be only one material with this id
    {
      throw vmat::NonUniqueMaterialException(material_id);
      return false;
    }
    else if (result.size() == 0) return false;
    else return true;
  }  
  // ---------------------------------------------------------------------------

  // -- Parameter Accessors ----------------------------------------------------
private:
  Entries queryParameter(std::string const& material_id, std::string const& parameter_id)
  {
    vars.set(vmat::key::id.c_str(),        material_id.c_str());  
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter->evaluate_node_set(mdb.xml);
  }
public:
  Entry getParameter(std::string const& material_id, std::string const& parameter_id)
  {
    pugi::xpath_node_set const& result = this->queryParameter(material_id, parameter_id);
    if(result.size() > 1) // there must be only one parameter with this id
    {
      throw vmat::NonUniqueParameterException(parameter_id);
      return Entry();
    }
    else if (result.size() == 0) return Entry();
    else return result.first(); // if success, return only a single xpath_node
  }

  bool hasParameter(std::string const& material_id, std::string const& parameter_id)
  {
    pugi::xpath_node_set const& result = this->queryParameter(material_id, parameter_id);
    if(result.size() > 1) // there must be only one material with this id
    {
      throw vmat::NonUniqueParameterException(parameter_id);
      return false;
    }
    else if (result.size() == 0) return false;
    else return true;
  }  

  vmat::Numeric getParameterValue(std::string const& material_id, std::string const& parameter_id)
  {
    vars.set(vmat::key::id.c_str(),        material_id.c_str());  
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter_value->evaluate_number(mdb.xml);
  }

  vmat::String getParameterUnit(std::string const& material_id, std::string const& parameter_id)
  {
    vars.set(vmat::key::id.c_str(),        material_id.c_str());  
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter_unit->evaluate_string(mdb.xml);
  }

  vmat::String getParameterNote(std::string const& material_id, std::string const& parameter_id)
  {
    vars.set(vmat::key::id.c_str(),        material_id.c_str());  
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter_note->evaluate_string(mdb.xml);
  }

  // ---------------------------------------------------------------------------
  
private:

  MaterialDatabase mdb;
  
  pugi::xpath_variable_set    vars;
  
  pugi::xpath_query *query_material;
  pugi::xpath_query *query_category;
  pugi::xpath_query *query_parameter;  
  pugi::xpath_query *query_parameter_value;
  pugi::xpath_query *query_parameter_unit;
  pugi::xpath_query *query_parameter_note;  
  
};

} //namespace vmat  

#endif


