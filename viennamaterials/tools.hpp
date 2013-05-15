#ifndef VIENNAMATERIALS_TOOLS_HPP
#define VIENNAMATERIALS_TOOLS_HPP

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
  void printToStream(Entries const& nodeset, std::ostream& stream = std::cout)
  {
    for(size_t i = 0; i < nodeset.size(); i++)
      nodeset[i].node().print(stream, "  ");
  }

  void printToStream(Entry const& node, std::ostream& stream = std::cout)
  {
    node.node().print(stream, "  ");
  }

  Entries query(Entry const& entry, std::string const& query)
  {
    return entry.node().select_nodes(query.c_str());
  }

  bool isParameter(Entry const& entry)
  {
    if(std::string(entry.node().name()) == vmat::key::parameter)
      return true;
    else return false;
  }

  bool isMaterial(Entry const& entry)
  {
    if(std::string(entry.node().name()) == vmat::key::material)
      return true;
    else return false;
  }

  vmat::Numeric value(Entry const& entry)
  {
    return pugi::xpath_query("value").evaluate_number(entry);
  }

  vmat::String name(Entry const& entry)
  {
    return pugi::xpath_query("name").evaluate_string(entry);
  }

  vmat::String unit(Entry const& entry)
  {
    return pugi::xpath_query("unit").evaluate_string(entry);
  }

  vmat::String note(Entry const& entry)
  {
    return pugi::xpath_query("note").evaluate_string(entry);
  }
} // end namespace vmat

#endif
