#ifndef VIENNAMATERIALS_PUGIXML_HPP
#define VIENNAMATERIALS_PUGIXML_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "library.hpp"

// ViennaUtils includes
//
#include "viennautils/xml.hpp"
#include "viennautils/file.hpp"

namespace viennamaterials {


/** 
    @brief Provides the pugixml backend in the dynamic class hierarchy
*/
class pugixml : public library
{
protected:
  typedef viennautils::xml<viennautils::tag::pugixml>::type     DatabaseType;
  typedef DatabaseType::NodeSet                                 EntriesType;
  typedef DatabaseType::Node                                    EntryType;
  typedef DatabaseType::NodeIterator                            EntryIteratorType;

public:
  pugixml();
  pugixml(std::string const& filename);
  ~pugixml();

  bool load(std::string const& filename);

  bool load(std::stringstream & stream);

  void dump(std::ostream& stream = std::cout);

  bool has_parameter(std::string const& material, std::string const& parameter);

  numeric_type get_parameter_value(std::string const& material, std::string const& parameter);

  std::string get_parameter_unit(std::string const& material, std::string const& parameter);

  keys_type get_materials_of_category(std::string const& category);

  bool has_materials_of_category(std::string const& category);

private:
  void init();
//  EntriesType queryParameter(EntryType const& material, std::string const& parameter_id);

private:
  DatabaseType                database_;

  pugi::xpath_variable_set    vars_;
  
  pugi::xpath_query *query_material_;
  pugi::xpath_query *query_category_;
  pugi::xpath_query *query_parameter_;
  pugi::xpath_query *query_parameter_value_;
  pugi::xpath_query *query_parameter_unit_;
  pugi::xpath_query *query_parameter_note_;

};

} // viennamaterials

#endif

