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

#include <sstream>

#include "viennamaterials/library.hpp"
#include "external/pugixml/pugixml.hpp"

#ifdef VIENNAMATERIALS_HAS_SERIALIZATION
  #include "boost/archive/text_iarchive.hpp"
  #include "boost/archive/text_oarchive.hpp"
#endif

namespace viennamaterials {


/** 
    @brief Provides the pugixml backend in the dynamic class hierarchy
*/
class pugixml : public library
{
private:
  typedef pugi::xpath_node_set      node_set_type;
  typedef pugi::xpath_node          node_type;
  typedef const pugi::xpath_node *  node_iterator_type;

public:
  pugixml();
  pugixml(std::string const& filename);
  ~pugixml();

  bool read(std::string const& filename);

  bool read(std::stringstream & stream);

  bool write(std::stringstream& stream);

  bool write(std::ofstream& ostream);

  bool write(std::string const& filename);

  void dump(std::ostream& stream = std::cout);

  bool has_parameter(std::string const& material, std::string const& parameter);

  viennamaterials::numeric get_parameter_value(std::string const& material, std::string const& parameter);

  std::string get_parameter_unit(std::string const& material, std::string const& parameter);

  viennamaterials::keys get_materials_of_category(std::string const& category);

  bool has_materials_of_category(std::string const& category);

#ifdef VIENNAMATERIALS_HAS_SERIALIZATION
  friend class boost::serialization::access;

  template<class Archive>
  void save(Archive & ar, const unsigned int version) const
  {
    std::stringstream sstream;
    xml_.save(sstream);
    std::string xmlstring= sstream.str();
    ar & xmlstring; 
  }

  template<class Archive>
  void load(Archive & ar, const unsigned int version)
  {
    std::string xmlstring;
    ar & xmlstring;
    std::stringstream sstream(xmlstring);
    this->read(sstream);
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif

private:
  void init();
  node_set_type query_parameter(std::string const& material, std::string const& parameter);
  std::string id(node_type const& entry);

private:
  pugi::xpath_variable_set    vars_;

  pugi::xpath_query *query_material_;
  pugi::xpath_query *query_category_;
  pugi::xpath_query *query_parameter_;
  pugi::xpath_query *query_parameter_value_;
  pugi::xpath_query *query_parameter_unit_;
  pugi::xpath_query *query_parameter_note_;

  pugi::xml_document    xml_;
  std::string           indent_string_;
};

} // viennamaterials

#endif

