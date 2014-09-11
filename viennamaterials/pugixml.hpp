#ifndef VIENNAMATERIALS_PUGIXML_HPP
#define VIENNAMATERIALS_PUGIXML_HPP

/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// System includes
//
#include <sstream>

// ViennaMaterials includes
//
#include "viennamaterials/backend.hpp"
#include "external/pugixml/pugixml.hpp"

namespace viennamaterials {


/**
    @brief Provides the pugixml backend
*/
class pugixml : public backend
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

  /** @brief Checks whether the query path is available in the database */
  bool has_entry(std::string const& native_query);

  /** @brief Perform a native query without conversion */
  std::string               query (std::string const& native_query);

  /** @brief Access the numeric value pointed to by the native query path */
  viennamaterials::numeric  query_value  (std::string const& native_query);
  
  /** @brief Perform a native query of type number */
  double                    query_xpath_number(std::string const& native_query);

  /** @brief Perform a native query of type string */
  std::string               query_xpath_string(std::string const& native_query);

  /** @brief Access the number of elements pointed by the native query path */
  long                      query_number_of_elements(std::string const& native_query);

  /** @brief Access the number of attributes of the element pointed by the native query path */
  long                      query_number_of_attributes(std::string const& native_query);

  /** @brief Access attribute name at given position of the element pointed by the native query path */
  std::string               query_attribute_name(std::string const& native_query, long const& position);

  /** @brief Access attribute value at given position of the element pointed by the native query path */
  std::string               query_attribute(std::string const& native_query, long const& position);

  /** @brief Access attribute value from attribute of given name of the element pointed by the native query path */
  std::string               query_attribute(std::string const& native_query, std::string const& attribute_name);

private:
  void                      init();
  bool                      is_valid_xml_index(long index);

private:
  pugi::xml_document    xml_;
  std::string           indent_string_;
};

} // viennamaterials

#endif
