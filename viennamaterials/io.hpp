#ifndef VIENNAMATERIALS_IO_HPP
#define VIENNAMATERIALS_IO_HPP

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

namespace vmat {


void printToStream(Entries const& nodeset, std::ostream& stream = std::cout)
{
  for(size_t i = 0; i < nodeset.size(); i++)
    nodeset[i].node().print(stream, "  ");
}

void printToStream(Entry const& node, std::ostream& stream = std::cout)
{
  node.node().print(stream, "  ");
}

} // end vmat
 
 
 
 
#endif
