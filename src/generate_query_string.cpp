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
#include "viennamaterials/library.hpp"

#include "boost/algorithm/string/replace.hpp"

namespace viennamaterials {

viennamaterials::string generate_query_string(viennamaterials::library * matlib, viennamaterials::query const& some_query)
{
  viennamaterials::string result;
  for(viennamaterials::query::const_iterator iter = some_query.begin();
      iter != some_query.end(); iter++)
  {
    viennamaterials::string path = matlib->get_accessor(iter->first)();
    boost::algorithm::replace_first(path, matlib->placeholder(), iter->second);
    result += path;
  }
  return result;
}

viennamaterials::string generate_query_string(library_handle& matlib, viennamaterials::query const& some_query)
{
  return viennamaterials::generate_query_string(matlib.get(), some_query);
}

} // viennamaterials
