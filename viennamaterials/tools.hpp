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
  Entries query(Entry const& entry, std::string const& query)
  {
    return entry.node().select_nodes(query.c_str());
  }

//  bool isParameter(Entry const& entry)
//  {
////    Entries result = vmat::query(entry, ".//
//  }

//  bool isMaterial(Entry const& entry)
//  {
//  }

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
