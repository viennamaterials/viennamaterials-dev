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

namespace vmat { 



struct Library 
{
  Library() 
  {
    // precompile the various query strings - use placeholders for run-time-specifics
    vars.add(vmat::key::id.c_str(), pugi::xpath_type_string);
    std::string material_query_string = "/materials/material[id = string($"+vmat::key::id+")]";
    query_material = new pugi::xpath_query(material_query_string.c_str(), &vars);
    
    vars.add(vmat::key::category.c_str(), pugi::xpath_type_string);
    std::string category_query_string = "/materials/material[category = string($"+vmat::key::category+")]";
    query_category = new pugi::xpath_query(category_query_string.c_str(), &vars);
    
    vars.add(vmat::key::parameter.c_str(), pugi::xpath_type_string);
    std::string parameter_query_string = "/material/parameters/parameter[name = string($"+vmat::key::parameter+")]";
    query_parameter = new pugi::xpath_query(parameter_query_string.c_str(), &vars);    
  }
  
  ~Library()
  {
    // the xpath queries are due to pugixml's impelmentation required to be allocated
    // on the heap ..
    delete query_material;
    delete query_category;
    delete query_parameter;
  }

  bool load(std::string const& filename)
  {
    if(!viennautils::file_exists(filename)) return false;
    
    if(viennautils::file_extension(filename) == "xml")  // native
    {
      mdb.read(filename);
      return true;
    }
    else if(viennautils::file_extension(filename) == "ipd")  // conversion required
    {
      ipdInit(NULL, NULL);
      
      // todo: 
      std::cout << "IPD support is not yet implemented .." << std::endl;
      
      return true;
    }
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

  Entries getMaterial(std::string const& material_id)
  {
    vars.set(vmat::key::id.c_str(), material_id.c_str());
    return query_material->evaluate_node_set(mdb.xml);
  }
  
  bool hasMaterial(std::string const& material_id)
  {
    return !(this->getMaterial(material_id).empty());
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

  Entries getParameter(Entries const& materials, std::string const& parameter_id)
  {
    if(materials.size() != 1)
    {
      std::cerr << "vmat::Library::getParameter: don't know which material to use .. " << std::endl;
      return Entries();
    }
    return this->getParameter(materials.first(), parameter_id);
  }

  Entries getParameter(Entry const& material, std::string const& parameter_id)
  {
//    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
//    return query_parameter->evaluate_node_set(material.node());

//    vmat::printToStream(material.node().select_nodes(".")); // works
    vmat::printToStream(material.node().select_nodes(".material"));

    
    return Entries();
  }

private:

  MaterialDatabase mdb;
  
  pugi::xpath_variable_set    vars;
  
  pugi::xpath_query *query_material;
  pugi::xpath_query *query_category;
  pugi::xpath_query *query_parameter;  
};

} //namespace vmat  

#endif


