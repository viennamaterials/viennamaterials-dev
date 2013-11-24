#ifndef VIENNAMATERIALS_WRITEQUERY_HPP
#define VIENNAMATERIALS_WRITEQUERY_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/forwards.h"

// Boost includes
//
#include "boost/algorithm/string/replace.hpp"

namespace viennamaterials {

template<typename MaterialLibraryPtrT>
void write_query(MaterialLibraryPtrT matlib, query& some_query, std::ostream& stream = std::cout)
{
  for(viennamaterials::query::const_iterator iter = some_query.begin(); 
      iter != some_query.end(); iter++)
  {
    std::string path = matlib->get_accessor(iter->first)();
    boost::algorithm::replace_first(path, matlib->placeholder(), iter->second);
    stream << path;
  }
  stream << "\n";
}

} // viennamaterials

#endif

