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

namespace viennamaterials {

void write_query(viennamaterials::library* matlib, query& some_query, std::ostream& stream)
{
  stream << viennamaterials::generate_query_string(matlib, some_query) << "\n";
}

void write_query(boost::shared_ptr<viennamaterials::library>& matlib, query& some_query, std::ostream& stream)
{
  stream << viennamaterials::generate_query_string(matlib, some_query) << "\n";
}

} // viennamaterials

