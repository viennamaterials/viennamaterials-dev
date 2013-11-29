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

namespace viennamaterials {


/** 
    @brief Provides the pugixml backend
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

  bool read(std::stringstream const& stream);

  bool write(std::stringstream& stream);

  bool write(std::ofstream& ostream);

  bool write(std::string const& filename);

  void dump(std::ostream& stream = std::cout);

  bool                      has_entry    (viennamaterials::query const& query);

  viennamaterials::string   query        (viennamaterials::query const& query);

  viennamaterials::numeric  query_value  (viennamaterials::query const& query);

  viennamaterials::string   query_native (viennamaterials::string const& native_query_string);

private:
  void                      init();
  viennamaterials::string   query_pugixml(viennamaterials::string const& native_query_string);

private:
  pugi::xml_document    xml_;
  std::string           indent_string_;
};

} // viennamaterials

#endif

